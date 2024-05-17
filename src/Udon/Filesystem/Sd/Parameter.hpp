//
//    SDカードにパラメータを保存するクラス
//
//    Copyright (c) 2023-2023 udonrobo
//
//    デフォルトではparameter.scvというファイルで保存される。読み取りと書き込みは同じファイル
//    小数のみ(10桁程度)
//    開発環境:Teensy4.1
//

#pragma once

#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)

#    include <SdFat.h>

namespace Udon
{

    class Parameter
    {
        SdFs        sd;
        FsFile      dataFile;
        int         size;
        const char* name;
        double*     readData;
        double*     writeData;
        bool        Card_Insertion;

    public:
        Parameter(int dataSize, const char* fileName = "parameter.csv")
            : size(dataSize)
            , name(fileName)
            , readData(new double[size]())
            , writeData(new double[size]())
            , Card_Insertion(sd.begin(SdioConfig(FIFO_SDIO)))
        {
        }
        // 書き込み
        void setParameter(int dataNum, double data)
        {
            writeData[dataNum - 1] = data;
        }
        void writeParameter()
        {
            if (Card_Insertion)
            {
                dataFile.open(name, O_RDWR);
                dataFile.seek(dataFile.size());    // ファイルの最後に移動
                for (int i = 0; i < size; i++)
                {
                    dataFile.print(writeData[i]);
                    dataFile.print(",");
                }
                dataFile.println();
                dataFile.close();
            }
        }
        // 読み取り
        void readParameter()
        {
            if (Card_Insertion)
            {
                dataFile.open(name, O_RDWR);
                String str;
                while (dataFile.available())
                {
                    str = dataFile.readStringUntil('\n', size * 15);
                }
                char c[size * 15];
                str.toCharArray(c, size * 15);
                char    tmp[15];
                int32_t i = 0, j = 0, k = 0;
                while (k < size)
                {
                    if (c[i] == '\0' || c[i] == ',')
                    {
                        tmp[j]        = '\0';
                        readData[k++] = atof(tmp);
                        j             = 0;
                    }
                    else
                        tmp[j++] = c[i];
                    i++;
                }
                dataFile.close();
            }
            else
            {
                for (int i = 0; i < size; i++)
                    readData[i] = 0.0;
            }
        }
        double getParameter(int dataNum) const
        {
            return readData[dataNum - 1];
        }
        void readShow(char end = {}) const
        {
            for (int i = 0; i < size; i++)
            {
                Serial.print(readData[i]);
                Serial.print("\t");
            }
            Serial.print(end);
        }
        void writeShow(char end = {}) const
        {
            for (int i = 0; i < size; i++)
            {
                Serial.print(writeData[i]);
                Serial.print("\t");
            }
            Serial.print(end);
        }
    };
}    // namespace Udon

#endif