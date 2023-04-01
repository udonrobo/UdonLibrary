# Delegator

メンバ関数をコールバック関数として登録できるようにするクラス

TwoWire クラスの onReceive のような高階関数の引数に静的でないメンバ関数は登録できません。本クラスはメンバ関数を呼ぶ静的メンバ関数を提供します。

## Data

-   file

    [Delegator.hpp](../src/udon/algorithm/Delegator.hpp)

-   依存ファイル

    STL: `vector`

-   名前空間

    `udon`

## Feature

- 開発経緯

	例えば次のような i2c 通信を使用したスレーブ側の受信クラスを考えます。一見あってそうに見えますが、このコードはエラーになります。`I2cSlaveReader::receiveEvent` 関数が静的関数でないため、`TwoWire::onReceive` 関数の引数に登録することができないからです。

	そこで`I2cSlaveReader::receiveEvent` を静的関数にするとなるわけですが、静的関数から通常のメンバ変数にアクセスすることはできないため、`buffer` も静的変数にする必要が出てきます。すると、`I2cSlaveReader` クラスはインスタンス間を複数作成しても `buffer` はインスタンス間で共有されるため複数インスタンス間を作ることができなくなってしまいます。

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


## Usage

-   基本

    ```
