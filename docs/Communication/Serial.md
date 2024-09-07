# UART / USB Serial

主に PC,マイコン間での通信に用います。この場合は USB で接続します。

## マイコンから PC

### マイコン側

`setMessage` 関数が送信処理を行うため、高周期で `setMessage` を呼び出すと PC にデータが送られすぎます。適度な delay が必要です。

```cpp
#include <Udon.hpp>

static Udon::SerialWriter<uint32_t> writer{ Serial };

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    writer.setMessage(millis());
    delay(10);
}
```

### PC 側 (OpenSiv3D が必要)

```cpp
#include <Siv3D.hpp> // Siv3D v0.6.15
#include <Udon.hpp>

void Main()
{
    Serial serial;
    Udon::SivSerialReader<uint32_t> reader{ serial };

    while (System::Update())
    {
        if (not serial)
        {
            serial.open(U"COM10", 115200);  // マイコンが接続されているCOMポートを指定
        }

        if (const auto m = reader.getMessage())
        {
            Print << *m;
        }
        else
        {
            Print << U"no message";
        }
    }
}
```

## PC からマイコン

### PC 側 (OpenSiv3D が必要)

```cpp
#include <Siv3D.hpp> // Siv3D v0.6.15
#include <Udon.hpp>

void Main()
{
    Serial serial;
    Udon::SivSerialWriter<uint32_t> writer{ serial };

    while (System::Update())
    {
        if (not serial)
        {
            serial.open(U"COM10", 115200);  // マイコンが接続されているCOMポートを指定
        }
        writer.setMessage(Time::GetMicrosec());
	}
}
```

### マイコン側

```cpp
#include <Udon.hpp>

Udon::SerialReader<uint32_t> reader{ Serial };

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if (const auto m = reader.getMessage())
    {
        uint32_t value = *m;
    }
    delay(10);
}
```

## OpenSiv3D COMポート列挙方法

`System::EnumerateSerialPorts` 関数から、接続されているシリアルポート情報 `SerialPortInfo` の配列を取得できます。例では map 関数を使って String の配列に加工しています。

```cpp
void Main()
{
    const Array<String> ports = System::EnumerateSerialPorts().map([](const SerialPortInfo& info)
    {
        return info.port;
    });

    Print << ports;

    while (System::Update()) {}
}
```