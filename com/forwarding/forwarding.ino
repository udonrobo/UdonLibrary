/// 構想

template<size_t N>
struct Im920Writer {
	uint8_t buffer[N];
	uint8_t* data() {
		return buffer;
	};
	void update() {}
	void init(int) {
	}
};

template<size_t N>
struct I2cSlaveReader {
	uint8_t buffer[N];
	const uint8_t* data() const {
		return buffer;
	};
	void update() {}
};

/// データ変換クラス(パイプみたいなもの)
/// --FromDataClass--> Forwarding --ToDataClass-->
template <
    template< size_t > class From,  /// 受信器
    template< size_t > class To  ,  /// 送信器
    size_t N                        /// データサイズ
    >
struct Forwarding {
	From<N> from;
	To  <N> to  ;
	void update() {
		from.update();
		memcpy(to.data(), from.data(), N);
		to.update();
	}
};

/// im920がPCに向けてデータ(10byte)を流す
/// I2cでデータを受け取り920MHz無線でデータを飛ばす
Forwarding<I2cSlaveReader, Im920Writer, 10> toPc;

/// PC側im920では次のような構成が考えられる
//Forwarding<Im920Reader, SerialWriter, 10> toPc;

void setup() {
	toPc.to.init(1212);
}

void loop() {
	toPc.update();
}
