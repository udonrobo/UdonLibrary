# 通信共通部

通信クラスのAPIを大体統一しているため、通信方式によらず同じようなプログラムを書くことができます。

## 共通内容

- 構造体等のオブジェクトを用いて通信を行います。

- 通信を管理するバスクラス、バスクラスとデータの受け渡しを行う送受信クラスから構成されています。

- 送受信クラスはテンプレート引数を持っており、通信するオブジェクトの型を指定します。

- 送信クラスの `setMessage(T)` を用いて送信し、受信クラスの `Optional<T> getMessage()` を用いて受信します。

## 送信者イメージ

```cpp
バスクラス bus;
送信クラス<int> writer{ bus, 0x01 };  // 送信先通信バスを登録

void setup()
{
    bus.begin();  // 通信開始
}

void loop()
{
    bus.update();  // 更新
    writer.setMessage(1000);  // バスへデータを送信
}
```

## 受信者イメージ

```cpp
バスクラス bus;
受信クラス<int> reader{ bus, 0x01 };  // 受信元通信バスを登録

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    if (const Optional<int> message = reader.getMessage())  // バスからデータを受信
    {
        Serial.println(*message);  // 受信成功 > 1000
    }
    else
    {
        Serial.println("receive failed");　// 受信エラー
    }
}
```

## 複数の通信バスや送受信者が存在する場合

バスクラスと送信者クラスに分けたことで、通信クラスを柔軟に組み合わせることができます。

通信によっては複数のバスや送受信者を立てることができない場合があります。

### 複数送信者

```mermaid
flowchart LR
    subgraph ファームウエア
        subgraph writer1
        送信オブジェクト1[オブジェクト] --int--> setMessage1[setMessage]
        end

        subgraph writer2
        送信オブジェクト2[オブジェクト] --int--> setMessage2[setMessage]
        end

        setMessage1[setMessage] --byte string--> bus
        setMessage2[setMessage] --byte string--> bus
    end

    bus -.byte string.-> 通信バス
```

```cpp
バスクラス bus;
送信クラス<int> writer1{ bus, 0x01 };
送信クラス<int> writer2{ bus, 0x02 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();
    writer1.setMessage(1000);
    writer2.setMessage(2000);
}
```

### 送受信者

```mermaid
flowchart LR
    subgraph ファームウエア
        subgraph writer1
        送信オブジェクト[オブジェクト] --int--> setMessage
        end

        subgraph writer2
        getMessage --int--> 受信オブジェクト[オブジェクト] 
        end

        setMessage --byte string--> bus
        
        direction RL
        bus --byte string--> getMessage
    end

    bus <-.byte string.-> 通信バス
```

```cpp
バスクラス bus;
送信クラス<int> writer{ bus, 0x01 };
受信クラス<int> reader{ bus, 0x02 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    writer.setMessage(1000);

    if (const auto message = reader.getMessage())
    {
        Serial.println(*message);
    }
    else
    {
        Serial.println("receive failed");
    }
}
```

### 複数バス、複数送受信

```mermaid
flowchart LR
    subgraph ファームウエア1
        subgraph writer2
        送信オブジェクト1[オブジェクト] --int--> setMessage1[setMessage]
        end

        subgraph writer3
        送信オブジェクト2[オブジェクト] --int--> setMessage2[setMessage]
        end

        subgraph writer1
        送信オブジェクト3[オブジェクト] --int--> setMessage3[setMessage]
        end

        setMessage3 --byte string--> ファームウエア1bus1
        setMessage1 --byte string--> ファームウエア1bus2
        setMessage2 --byte string--> ファームウエア1bus2
    end

    ファームウエア1bus1[bus1] -.byte string.-> 通信バス1[通信バス]
    ファームウエア1bus2[bus2] -.byte string.-> 通信バス2[通信バス]


    通信バス1[通信バス] -.byte string.-> ファームウエア2bus1[bus1]
    通信バス2[通信バス] -.byte string.-> ファームウエア2bus2[bus2]

    subgraph ファームウエア2
        subgraph reader1
        getMessage1 --int--> 受信オブジェクト1[オブジェクト]
        end

        subgraph reader2
        getMessage2 --int--> 受信オブジェクト2[オブジェクト]
        end

        subgraph reader3
        getMessage3 --int--> 受信オブジェクト3[オブジェクト]
        end

        ファームウエア2bus1 --byte string--> getMessage1
        ファームウエア2bus2 --byte string--> getMessage2
        ファームウエア2bus2 --byte string--> getMessage3
    end
```

```cpp
バスクラス bus1;
バスクラス bus2;

送信クラス<int> writer1{ bus1, 0x01 };
送信クラス<int> writer2{ bus2, 0x01 };
送信クラス<int> writer3{ bus2, 0x02 };

void setup()
{
    bus1.begin();
    bus2.begin();
}

void loop()
{
    bus1.update();
    bus2.update();
    writer1.setMessage(1000);
    writer2.setMessage(2000);
    writer3.setMessage(3000);
}
```

```cpp
バスクラス bus1;
バスクラス bus2;

受信クラス<int> reader1{ bus1, 0x01 };
受信クラス<int> reader2{ bus2, 0x01 };
受信クラス<int> reader3{ bus2, 0x02 };

void setup()
{
    bus1.begin();
    bus2.begin();
}

void loop()
{
    bus1.update();
    bus2.update();

    if (const auto message = reader1.getMessage())
    {
        Serial.println(*message);
    }
    else
    {
        Serial.println("receive failed");
    }

    if (const auto message = reader2.getMessage())
    {
        Serial.println(*message);
    }
    else
    {
        Serial.println("receive failed");
    }

    if (const auto message = reader3.getMessage())
    {
        Serial.println(*message);
    }
    else
    {
        Serial.println("receive failed");
    }
}
```
