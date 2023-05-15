# Delegate

関数ポインタからメンバ関数を呼び出す

## Description

メンバ関数をコールバック関数として登録できるようにするクラス

TwoWire クラスの onReceive のような高階関数は関数ポインタを引数にとるため、静的でないメンバ関数は登録できません。

本クラスはメンバ関数を呼ぶ静的メンバ関数を提供します。C#の delegate を参考にしています。

注意！！大抵のケースでこのクラスを使用せず実装することができます。

<details>
<summary>大抵のケース</summary>

1. 高階関数の引数に void ポインタ等を渡せるようになっている場合。(WindowsAPI 等は渡せます)

    ```cpp
    class Sample
    {
    	int value;
    public:
    	Sample(Receiver& receiver)
    	{
    		receiver.onReceive(onReceive, this);
    	}
    	static void onReceive(void* p)  // pにvoidポインタにキャストされたthisポインタがわたる(ライブラリ側が渡してくれる)
    	{
    		auto self = static_cast<Sample*>(p);  // voidポインタからthisポインタに復元(?)
    		self->value = 1234;  // 通常メンバにアクセスできる ﾔｯﾀｰｰｰｰ
    	}
    };
    ```

2. 高階関数の引数に関数オブジェクト(std::function 等)を渡せるようになっている場合。

    ```cpp
    class Sample
    {
    	int value;
    public:
    	Sample(Receiver& receiver)
    	{
    		receiver.onReceive([this]() {  // thisポインタをキャプチャ
    			value = 123;
    		});
    	}
    };
    ```

3. ライブラリを作るときは void ポインタを渡せるようになってるとおじさん嬉しい 🥲

 </details> <br>

テンプレートや、SFINAE を使用するためコンパイル時間がモリモリ伸びちゃいます。

## Data

-   file

    [Delegate.hpp](../../src/udon/algorithm/Delegate.hpp)

-   名前空間

    `udon`

## Usage

1. 本クラスが含まれるファイルをインクルードします。

    ```cpp
    #include <Udon.hpp>
    // or
    #include <udon/algorithm/Delegate.hpp>
    ```

2. 登録したいメンバ関数があるクラス内でインスタンス化します。

    `Delegate` クラスのテンプレート引数には、メンバ関数があるクラス、メンバ関数の戻り値の型、引数の型を指定します。

    コンストラクタの引数には、メンバ関数があるクラスのインスタンスを指すポインタ(this ポインタ)、メンバ関数ポインタを指定します。

    ```cpp
    #include <udon/algorithm/Delegate.hpp>
    class Sample
    {
    	udon::Delegate<Sample, void(int)> delegate;
    public:
    	Sample()
    		: delegate(this, &Sample::onReceive)
    	{
    	}
    	void onReceive(int)
    	{
    		// do something
    	}
    };
    ```

3. 呼び出す、関数ポインタに変換する

    2 で登録したメンバ関数は `Delegate::Execute` 静的メンバ関数を呼ぶことで呼び出せます。

    ```cpp
    Delegate<Sample, void(int)>::Execute(123);
    ```

    また 関数ポインタにキャストしたときに `Delegate::Execute` を取得することができる `Delegate::operator CFunctor()` を使用して呼び出すこともできます。

    ```cpp
    delegate(123);
    Wire.onReceive(delegate);
    ```

-   留意点

    注意点として、`Delegate` クラスはインスタンスポインタ、メンバ関数ポインタを静的変数で保持します。

    そのため、同じ型の `Delegate` クラスにつき一つのメンバ関数しか登録できません。(変数を共有するため)

    ```cpp
      class Sample
      {
      	udon::Delegate<Sample, void(int)> delegate0;
      	udon::Delegate<Sample, void(int)> delegate1;
      public:
      	Sample()
      		: delegate0(this, &Sample::onReceive)
      		, delegate1(this, &Sample::onRequest)  // delegate0に登録しているメンバ関数を上書き🫠🫠
      	{}
      	void onReceive(int) {}
    	void onRequest(int) {}
      };
    ```

    `Delegate` クラスの第三テンプレート引数にユニークな値を指定することで `udon::Delegate<Sample, void(int), 0>` `udon::Delegate<Sample, void(int), 1>` はそれぞれ異なる型となるので保持することができます。(ゴリ押し)

    `__COUNTER__` マクロを使うことでユニークな値を生成できます。

    ```cpp
      class Sample
      {
      	// udon::Delegate<Sample, void(int), 0> delegate0;
      	// udon::Delegate<Sample, void(int), 1> delegate1;
      	udon::Delegate<Sample, void(int), __COUNTER__> delegate0;
      	udon::Delegate<Sample, void(int), __COUNTER__> delegate1;
      public:
      	Sample()
      		: delegate0(this, &Sample::onReceive)
      		, delegate1(this, &Sample::onReceive)
      	{}
      	void onReceive(int) {}
    	void onRequest(int) {}
      };
    ```

    また `Delegate` を使用したクラスのインスタンスは一つしか生成することができません。(これもメンバ関数の上書き登録になる)

    さらにゴリ押すことで複数生成できるようになります。`__COUNTER__` マクロを使うため配列にすることはできません。<ここがダメ！！

    ```cpp
    template<int Unique>
      class Sample_impl
      {
      	udon::Delegate<Sample_impl, void(int)> delegate;
      public:
      	Sample_impl()
      		: delegate(this, &Sample_impl::onReceive)
      	{
      	}
      	void onReceive(int)
      	{
      		// do something
      	}
      };

    #define Sample Sample_impl<__COUNTER__>  // 😶‍🌫️😶‍🌫️😶‍🌫️😶‍🌫️

    Sample a;
    Sample b;
    ```

## Example

TwoWire クラスの高階関数にメンバ関数を登録する例

```cpp
#include <Wire.h>
#include <udon/algorithm/Delegate.hpp>

class I2cSlaveReader
{

	TwoWire& wire;

	udon::Delegate<I2cSlaveReader, void(int)> onReceiveDelegate;

public:

	I2cSlaveReader(TwoWire& wire)
		: wire(wire)
		, onReceiveDelegate(this, &I2cSlaveReader::onReceive)
	{
		wire.onReceive(onReceiveDelegate);
	}

	// 受信時にここが呼び出される
	void onReceive(int)
	{
		int data = wire.read();
		...
	}

};

I2cSlaveReader reader{ Wire };

void setup() {}
void loop() {}
```

## 開発経緯

例えば下のような i2c 通信を使用したスレーブ側の受信クラスを考えます。一見正しそうに見えますが、このコードはエラーになります。`I2cSlaveReader::onReceive` 関数が静的関数でないため、`TwoWire::onReceive` 関数の引数に登録することができないからです。

そこで`I2cSlaveReader::onReceive` を静的関数にするとなるわけですが、静的関数から通常のメンバ変数にアクセスすることはできないため、`buffer` も静的変数に..と静的変数が無限に増えていきます。この問題を解消するために `Delegate` を開発しました。

```cpp
class I2cSlaveReader
{

	TwoWire& wire;

	uint8_t buffer[--];

public:

	I2cSlaveReader(TwoWire& wire)
		: wire(wire)
	{
		wire.onReceive(onReceive);  // 🫠🫠🫠🫠
	}

	void onReceive(int)
	{
		while (wire.available())
		{
			for (auto&& it : buffer)
			{
				it = wire.read();
			}
		}
	}

};

I2cSlaveReader reader { Wire };
```
