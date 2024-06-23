# I2C 通信

I2C 通信は主に基板内で使用されるシリアル通信の一種で、一対多の通信方式です。バス型方式をとっているため容易に配線が可能です。I2C 通信用端子はマイコンに標準で搭載されている場合が多く、導入コストが低いです。一方で配線長が長くなると規格以上の配線容量となってしまい通信エラーが発生しやすくなります。

通信タイミングの管理を行うマイコンはマスターと呼ばれます。マスターと通信するマイコンはスレーブと呼ばれます。

I2C 通信クラスは、通信バスクラス、マスター側送受信クラス、スレーブ側送受信クラスから構成されています。

## 個別インクルード

```cpp
#include <Udon/Com/I2c.hpp>
```

## バスクラス

`Udon::I2cBus`

Arduino の `TwoWire` クラスへ通信タイムアウト時にバスの再起動を行う機能を追加したバスクラスです。

通信開始時、マスターモードの場合 `begin(void)`、スレーブモードの場合 `begin(address)` を呼び出す点に注意してください。

## マスターからスレーブへ送信

### マスター送信クラス

`Udon::I2cMasterWriter<T>`

`T` に指定された型のオブジェクトをスレーブへ送信します。スレーブの数に応じて複数インスタンス化可能です。

```cpp
#include <Udon.hpp>

static Udon::I2cBus bus{ Wire };
static Udon::I2cMasterWriter<Udon::Vec2> writer{ bus, 6 };   // スレーブアドレス 6 へ送信

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    writer.setMessage({ millis(), micros() });
    writer.update();

    delay(10);
}
```

### スレーブ受信クラス

`Udon::I2cSlaveReader<T>`

`T` に指定された型のオブジェクトをマスターから受信します。複数インスタンス化できません。

```cpp
#include <Udon.hpp>

static Udon::I2cBus bus{ Wire };
static Udon::I2cSlaveReader<Udon::Vec2> reader{ bus };

void setup()
{
    Serial.begin(115200);

    bus.begin(6);  // 自身のアドレス
    reader.begin();
}

void loop()
{
    bus.update();

    if (const auto message = reader.getMessage())
    {
        message->show();
    }
    else
    {
        Serial.print("receive failed");
    }
    Serial.println();

    delay(10);
}
```

## スレーブからマスターへ送信

### スレーブ送信クラス

`Udon::I2cSlaveWriter<T>`

`T` に指定された型のオブジェクトをマスターへ送信します。複数インスタンス化できません。

```cpp
#include <Udon.hpp>

static Udon::I2cBus bus{ Wire };
static Udon::I2cSlaveWriter<Udon::Vec2> writer{ bus };

void setup()
{
    bus.begin(6);  // 自身のアドレス
    writer.begin();
}

void loop()
{
    bus.update();

    writer.setMessage({ millis(), micros() });

    delay(10);
}
```

### マスター受信クラス

`Udon::I2cMasterReader<T>`

`T` に指定された型のオブジェクトをスレーブから受信します。スレーブの数に応じて複数インスタンス化可能です。

```cpp
#include <Udon.hpp>

static Udon::I2cBus bus{ Wire };
static Udon::I2cMasterReader<Udon::Vec2> reader{ bus, 6 };  // スレーブアドレス 6 から受信

void setup()
{
    Serial.begin(115200);
    bus.begin();
}

void loop()
{
    bus.update();

    reader.update();
    if (const auto message = reader.getMessage())
    {
        message->show();
    }
    else
    {
        Serial.print("receive failed");
    }
    Serial.println();

    delay(10);
}
```
