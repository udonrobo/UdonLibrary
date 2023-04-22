#include<Udon.hpp>
using udon::parameter;

const int dataSize = 10;
const char* fileName = "parameter.csv";

parameter myParameter(dataSize, fileName);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myParameter.readParameter();
  myParameter.readShow();
  for (int i = 0; i < 10; i++)
    myParameter.setParameter(i + 1, random(100));
  myParameter.writeShow();
  myParameter.writeParameter();
}

void loop() {
  // put your main code here, to run repeatedly:

}
