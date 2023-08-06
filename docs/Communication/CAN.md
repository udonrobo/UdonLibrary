# CAN 通信

## Usage

1.  ファイルインクルード

    ```cpp
    #include <Udon.hpp>
    // or
    #include <Udon/Com/Can.hpp>
    ```

2.  インスタンス化

    CAN 通信用のクラスは、CAN バス管理クラス、送受信ノードクラスから構成されています。

    - バス管理クラス

      通信が行えているかどうかのチェック、送受信処理を行います。

      このクラスのインスタンスは一つのバスにつき 1 つ作成します。使用するマイコンによって書き方が異なります。

      ```cpp
      // teensy4.0 使用時
      Udon::CanBusTeensy<CAN1> bus;

      // CANコントローラ使用時(SPI経由)
      Udon::CanBusSpi bus{ SPI };
      ```

    - 受信クラス

      通信バスから取得したバイト列をオブジェクトに復元し、取得できるようにします。

      `CanReader<T>` クラスを使用します。

      テンプレート引数には受信するオブジェクトの型を与えます。

      コンストラクタの第一引数にバス管理クラスのインスタンス、第二引数に受信したい送信ノードの ID を与えます。

      ```cpp
      Udon::CanReader<Udon::Vec2> reader{ bus, 10 };  // ノードID 10 からのデータを受信
      ```

      `getMessage()` を使用して受信したデータを取得します。この関数は `Udon::Optional<T>` を返します。通信エラー時は `Udon::nullopt` が返されます。`Udon::Optional` は `operator bool()` を持っているため if 文で分岐できます。

      ```cpp
      if (const Udon::Optional<Udon::Vec2> vector = reader.getMessage())
      {
          // 受信成功
      }
      else
      {
          // 受信失敗(データ破損検出、通信タイムアウト時等)
      }
      ```

      > 複数受信ノードを作成することもできます。
      >
      > ```cpp
      > Udon::CanReader<Udon::Vec2> reader1{ bus, 11 };  // ノードID 11 からのデータを受信
      > Udon::CanReader<Udon::Vec2> reader2{ bus, 12 };  // ノードID 12 からのデータを受信
      > ```

    - 送信クラス

      オブジェクトをバイト列に変換し通信バスへ送信します。

      `CanWriter<T>` クラスを使用します。

      テンプレート引数には送信するオブジェクトの型を与えます。

      コンストラクタの第一引数にバス管理クラスのインスタンス、第二引数に自身のノードの ID を与えます。

      ```cpp
      Udon::CanWriter<Udon::Vec2> reader{ bus, 10 };  // ノードID 10 としてデータ送信
      ```

      `setMessage(T)` を使用して送信したいオブジェクトをセットします。

      ```cpp
      Udon::Vec2 vector{ 100.0, 200.0 };
      writer.setMessage(vector);
      ```

      > 複数送信ノードを作成することもできます。
      >
      > ```cpp
      > Udon::CanWriter<Udon::Vec2> writer1{ bus, 11 };  // ノードID 11 としてデータ送信
      > Udon::CanWriter<Udon::Vec2> writer2{ bus, 12 };  // ノードID 12 としてデータ送信
      > ```
