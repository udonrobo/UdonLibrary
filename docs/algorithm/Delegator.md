# Delegator

メンバ関数をコールバック関数として登録できるようにするクラス

TwoWire クラスの onReceive のような高階関数の引数に静的でないメンバ関数は登録できません。本クラスはメンバ関数を呼ぶ静的メンバ関数を提供します。

## Data

-   file

    [Delegator.hpp](../../src/udon/algorithm/Delegator.hpp)

-   依存ファイル

    STL: `vector`

-   名前空間

    `udon`

<details>
<summary>開発経緯</summary>

	例えば下のような i2c 通信を使用したスレーブ側の受信クラスを考えます。一見正しそうに見えますが、このコードはエラーになります。`I2cSlaveReader::receiveEvent` 関数が静的関数でないため、`TwoWire::onReceive` 関数の引数に登録することができないからです。

	そこで`I2cSlaveReader::receiveEvent` を静的関数にするとなるわけですが、静的関数から通常のメンバ変数にアクセスすることはできないため、`buffer` も静的変数にする必要が出てきます。すると、`I2cSlaveReader` クラスはインスタンス間を複数作成しても `buffer` はインスタンス間で共有されるため複数インスタンス間を作ることができなくなってしまいます。この問題を解消するために `Delegator` を開発しました。

	```cpp
	class I2cSlaveReader
	{

		TwoWire& wire;

		uint8_t buffer[--];

	public:

		I2cSlaveReader(TwoWire& wire)
			: wire(wire)
		{
			wire.onReceive(receiveEvent);
		}

		void receiveEvent(int)
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

	I2cSlaveReader reader0 { Wire  };
	I2cSlaveReader reader1 { Wire1 };
	```
</details>

## Usage

-   `Delegator` クラス

	`Delegator` クラスを継承し、`Delegator::callback` をオーバーライドすることで、 `Delegator::Execute` を呼び出したときに `Delegator::callback` を呼び出すことができるようになります。

	`Delegator` のテンプレート引数には `Delegator::Execute` の戻り値と、引数の型を `Delegator<戻り値(引数...)>` のように指定します。`Delegator::callback` の戻り値と、引数の型もここで指定した型になります。

	```cpp
	#include <iostream>
	#include "Delegator.hpp"

	class Sample
		: udon::Delegator<void()>
	{

	public:

		void callback() override
		{
			std::cout << "call" << std::endl;
		}

	};

	Sample sample;

	int main()
	{
		udon::Delegator<void()>::Execute();
		// > call
	}
	```

	`Delegator::Execute` は静的メンバ関数ですから、下のように高階関数の引数に入れることもできます。

	```cpp
	Wire.onReceive(udon::Delegator<void(int)>::Execute);
	```

-   `MultipleDelegator` クラス

	`Delegator` クラスは一つのインスタンスの `Delegator::callback` しか呼び出すことができません。そのため次のようなコードでは思うような動作になりません。

	```cpp
	Sample sample0;
	Sample sample1;
	Sample sample2;

	int main()
	{
		udon::Delegator<void()>::Execute();
		// > call    <- sample2 の callback が呼ばれる😢
	}
	```

	そこで二つ以上のインスタンスの `Delegator::callback` を呼び出す場合は `MultipleDelegator` クラスを使用します。ただし、内部に動的配列を持つため注意が必要です。また戻り値を指定した場合に一番最後に登録された `Delegator::callback` の戻り値が返ってくる点も注意が必要です。

	```cpp
	#include <iostream>
	#include "Delegator.hpp"

	class Sample
		: udon::MultipleDelegator<void()>
	{

	public:

		void callback() override
		{
			std::cout << "call" << std::endl;
		}

	};

	Sample sample0;
	Sample sample1;
	Sample sample2;

	int main()
	{
		udon::MultipleDelegator<void()>::Execute();
		// > call
		// > call
		// > call
	}
	```

## Example

TwoWire クラスの高階関数にメンバ関数を登録する例

```cpp

#include <Wire.h>
#include <Udon.hpp>

// TwoWire インスタンスにつき一つしかインスタンスを生成しないので Delegator を使用する
class I2cSlaveReader
	: udon::Delegator<void(int)>
{

	TwoWire& wire;

public:

	I2cSlaveReader(TwoWire& wire)
		: wire(wire)
	{
		wire.onReceive(Delegator<void(int)>::Execute);
	}

	// 受信時にここが呼び出される
	void callback(int) override
	{
		//受信時処理
		hogehoge = wire.read();
	}

};

I2cSlaveReader reader0{ Wire  };
I2cSlaveReader reader1{ Wire1 };
I2cSlaveReader reader2{ Wire2 };

void setup() {}
void loop() {}
```