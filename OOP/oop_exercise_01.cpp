//Создать класс TimePoint для работы с моментами времени в формате «час:минута:секунда».
//Обязательными операциями являются : вычисление разницы между двумя моментами времени,
//сумма моментов времени, сложение момента времени и заданного количества секунд,
//вычитание из момента времени заданного количества секунд,
//вычисление во раз сколько один момент времени больше(меньше) другого, сравнение моментов времени,
//перевод в секунды и обратно, перевод в минуты(с округлением до минуты) и обратно.

//тесты находятся в файле data.txt в таком порядке: 1-ый момент времении, 2-ой момент времени, количество секунд, количество минут
//пример теста:
//22:13:05
//22:13:05
//25004
//1580

#include <string>
#include <iostream>
#include <fstream>

#define MAX_SECONDS 86399  ///для 24-ого формата времени

using namespace std;

class TimePoint {
public:
	void SetSeconds(const int new_seconds) { seconds = new_seconds; }

	TimePoint() { seconds = 0; }
	TimePoint(const int new_hours, const int new_minutes, const int new_seconds) {
		TranslateInSeconds(new_hours, new_minutes, new_seconds);
	}

	~TimePoint() {}

	int GetInSeconds() const { return seconds; }

	string Get() const {		
		return TranslateFromSeconds();
	}

private:
	void TranslateInSeconds(int hour, int minute, int second) {
		seconds = hour * 3600 + minute * 60 + second;
	}
	string TranslateFromSeconds() const {
		int hour = seconds / 3600;
		int minute = (seconds % 3600) / 60;
		int second = seconds - hour * 3600 - minute * 60;
		string h = to_string(hour);
		string m = to_string(minute);
		string s = to_string(second);
		string time = "";
		if (hour < 10) { time += '0'; }
		time += h;
		time += ':';
		if (minute < 10) { time += '0'; }
		time += m;
		time += ':';
		if (second < 10) { time += '0'; }
		time += s;
		return time;
	}

	int seconds;
};

string DifferenceMomentsTimes(const TimePoint& time1, const TimePoint& time2) {

	TimePoint times;

	if (time1.GetInSeconds() >= time2.GetInSeconds()) {
		times.SetSeconds(time1.GetInSeconds() - time2.GetInSeconds());
	}
	else {
		times.SetSeconds(time2.GetInSeconds() - time1.GetInSeconds());
	}
	string time_str = times.Get();
	return time_str;
}

string SumMomentsTimes(const TimePoint& time1, const TimePoint& time2) {

	TimePoint times;

	if (time1.GetInSeconds() + time2.GetInSeconds() >= MAX_SECONDS) {
		return "time overflow";
	}
	times.SetSeconds(time1.GetInSeconds() + time2.GetInSeconds());
	string time_str = times.Get();
	return time_str;
}

string DifferenceTimesAndSeconds(const TimePoint& time1, int new_seconds) {

	TimePoint times;

	if (time1.GetInSeconds() < new_seconds) {
		return "can't be difference";
	}
	times.SetSeconds(time1.GetInSeconds() - new_seconds);
	string time_str = times.Get();
	return time_str;
}


string SumTimesAndSeconds(const TimePoint& time1, int new_seconds) {

	TimePoint times;

	if (time1.GetInSeconds() + new_seconds >= MAX_SECONDS) {	
		return "Time overflow";
	}
	times.SetSeconds(time1.GetInSeconds() + new_seconds);

	string time_str = times.Get();
	return time_str;
}

string HowManyTimesMore(const TimePoint& time1, const TimePoint& time2) {

	TimePoint times;

	if (time1.GetInSeconds() >= time2.GetInSeconds()) {
		times.SetSeconds(time1.GetInSeconds() / time2.GetInSeconds());
		string ans = "First moment time more in ";
		ans += to_string(times.GetInSeconds());
		return ans;
	}

	times.SetSeconds(time2.GetInSeconds() / time1.GetInSeconds());
	string ans = "Second moment time more in ";
	ans += to_string(times.GetInSeconds());
	return ans;
}

string ComporisionMomentsTimes(const TimePoint& time1, const TimePoint& time2) {

	TimePoint times;

	if (time1.GetInSeconds() > time2.GetInSeconds()) {
		return "First time more than second time";
	}
	if (time2.GetInSeconds() > time1.GetInSeconds()) {
		return "Second time more than first time";
	}
	return "Times are equal";
}

int MomentTimeToSeconds(const TimePoint& time1) {
	return time1.GetInSeconds();
}


string SecondsToMomentTime(int new_seconds) {
	if (new_seconds > MAX_SECONDS) {
		return "time overflow";
	}
	TimePoint times;
	times.SetSeconds(new_seconds);
	string time_str = times.Get();
	return time_str;
}


int MomentTimeToMinutes(const TimePoint& time1) {
	int sec = time1.GetInSeconds();
	int minutes = sec / 60;
	sec -= minutes * 60;
	if (sec >= 30) {
		++minutes;
	}
	return minutes;
}


string MinutesToMomentTime(int new_minutes) {
	if (new_minutes * 60 > MAX_SECONDS) {
		return "time overflow";
	}
	TimePoint times;
	times.SetSeconds(new_minutes * 60);
	return times.Get();


}
bool IsHour(const int num) {
	if (num < 0 || num > 24) {
		return false;
	}
	return true;
}

bool IsMinOrSec(const int num) {
	if (num < 0 || num > 59) {
		return false;
	}
	return true;
}


int main() {

	const string path = "data.txt";
	ifstream input(path);
	int hours1, minutes1, seconds1, hours2, minutes2, seconds2;
	int sec, min;
	/*cin >> hours1 >> minutes1 >> seconds1;
	cin >> hours2 >> minutes2 >> seconds2;
	*/
	if (input.is_open()) {		
		input >> hours1;
		if (!IsHour(hours1)) { cout << "ERROR INPUT"; return 1; }
		input.ignore(1);
		input >> minutes1;
		if (!IsMinOrSec(minutes1)) { cout << "ERROR INPUT"; return 1; }
		input.ignore(1);
		input >> seconds1;
		if (!IsMinOrSec(seconds1)) { cout << "ERROR INPUT"; return 1; }

		input >> hours2;
		if (!IsHour(hours2)) { cout << "ERROR INPUT"; return 1; }
		input.ignore(1);
		input >> minutes2;
		if (!IsMinOrSec(minutes2)) { cout << "ERROR INPUT"; return 1; }
		input.ignore(1);
		input >> seconds2;
		if (!IsMinOrSec(seconds2)) { cout << "ERROR INPUT"; return 1; }


		input >> sec;		//ввод времени в секундах
		input >> min;		//ввод времени в минутах

		TimePoint time1(hours1, minutes1, seconds1);
		TimePoint time2(hours2, minutes2, seconds2);


		cout << DifferenceMomentsTimes(time1, time2) << " - Difference moments times" << endl;
		cout << SumMomentsTimes(time1, time2)		 << " - Moments times sum" << endl;

		cout << DifferenceTimesAndSeconds(time1, sec)<< " - Difference first times and seconds" << endl;
		cout << SumTimesAndSeconds(time1, sec)		 << " - Sum first times and seconds" << endl;

		cout << HowManyTimesMore(time1, time2) << endl;
		cout << ComporisionMomentsTimes(time1, time2) << endl;

		cout << MomentTimeToSeconds(time1)			 << " - Conversion of first time to seconds" << endl;
		cout << SecondsToMomentTime(sec)			 << " - Conversion of seconds to time" << endl;

		cout << MomentTimeToMinutes(time1)			 << " - Conversion of first time to minutes" << endl;
		cout << MinutesToMomentTime(min)			 << " - Conversion of minutes to time" << endl;
	}
	else { cout << "ERROR"; }

	return 0;
}





