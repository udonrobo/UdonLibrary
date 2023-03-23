#include "Can.hpp"

#include <vector>

CanBusTeensy<CAN1> bus;

std::vector<CanWriter<Message::Motor>> writers{
    { bus, 0 },
    { bus, 1 },
    { bus, 2 },
    { bus, 3 },
    { bus, 4 },
    { bus, 5 },
    { bus, 6 }
};

void setup()
{
    bus.begin();
}

void loop()
{
    for (auto&& writer : writers)
    {
        writer.setMessage({ (int16_t)millis() });
        if (writer)
        {
            writer.show();
        }
        else
        {
            Serial.println("error");
        }
    }
    Serial.println(bus.getErrorInfo());

    bus.update();
    delay(1);
}
