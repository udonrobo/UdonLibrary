#include <cstdint>
#include <functional>
#include <chrono>

struct Scheduler {
	using Functor = std::function<void(void)>;

	Scheduler* next;

	Functor   callback;
	bool      isRunning;
	long long startMs;

	/// @brief          スケジュール作成
	/// @param callback 自身のスケジュールの場合に呼ばれるファンクタ
	Scheduler(Functor&& callback) noexcept
		: next()
		, callback(std::move(callback))
		, isRunning()
		, startMs()
	{
	}

	/// スケジュール更新
	void update() {
		if (isRunning)
			callback();   /// 自身のスケジュールの場合実行
		if (next)
			next->update();
	}

	/// @brief スケジュール紐づけ
	Scheduler& operator>>(Scheduler& r) {
		next = &r;
		return r;
	}

	/// @brief 自身のスケジュールが開始されてからの経過時間[ms]
	long long millis() {
		return systemMillis() - startMs;
	}

	void finish() {
		isRunning = false;
		next->start();
	}
	void start() {
		isRunning = true;
		startMs = systemMillis();
	}

	long long systemMillis() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
};

#include <iostream>
#include <windows.h>

int main()
{
	Scheduler moveSequence{ [&] { std::cout << "start\n";  } };
	Scheduler a{ [&] { std::cout << a.millis() << std::endl;  } };
	Scheduler b{ [&] { std::cout << "b    \n";  } };
	Scheduler c{ [&] { std::cout << "c    \n";  } };

	moveSequence >> a >> b >> c;

	a.start();

	for (;;)
	{
		moveSequence.update();
		Sleep(100);
	}
}