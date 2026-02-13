#include <array>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

class PerpetualCalendar {
public:
    static constexpr int kMinYear = 1;
    static constexpr int kMaxYear = 3000;

    bool isLeapYear(int year) const {
        validateYear(year);
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    int daysInMonth(int year, int month) const {
        validateYear(year);
        validateMonth(month);

        static constexpr std::array<int, 12> kDays = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        if (month == 2 && isLeapYear(year)) {
            return 29;
        }
        return kDays[month - 1];
    }

    bool isValidDate(int year, int month, int day) const {
        if (year < kMinYear || year > kMaxYear) {
            return false;
        }
        if (month < 1 || month > 12) {
            return false;
        }
        if (day < 1 || day > daysInMonth(year, month)) {
            return false;
        }
        return true;
    }

    // 返回值: 0=周一, 1=周二, ..., 6=周日
    int dayOfWeek(int year, int month, int day) const {
        validateDate(year, month, day);

        long long days = daysBeforeYear(year) + daysBeforeMonth(year, month) + (day - 1);
        return static_cast<int>(days % 7);
    }

    int dayOfYear(int year, int month, int day) const {
        validateDate(year, month, day);
        return daysBeforeMonth(year, month) + day;
    }

    void printMonthCalendar(int year, int month) const {
        validateYear(year);
        validateMonth(month);

        static constexpr std::array<const char*, 12> kMonthNames = {
            "一月", "二月", "三月", "四月", "五月", "六月",
            "七月", "八月", "九月", "十月", "十一月", "十二月"
        };

        const int firstWeekday = dayOfWeek(year, month, 1);  // 0=周一
        const int totalDays = daysInMonth(year, month);

        std::cout << "\n========== " << year << " 年 " << kMonthNames[month - 1] << " ==========" << "\n";
        std::cout << "一  二  三  四  五  六  日\n";

        for (int i = 0; i < firstWeekday; ++i) {
            std::cout << "    ";
        }

        for (int day = 1; day <= totalDays; ++day) {
            std::cout << std::setw(2) << day << "  ";
            if ((firstWeekday + day) % 7 == 0) {
                std::cout << '\n';
            }
        }
        std::cout << "\n";
    }

private:
    void validateYear(int year) const {
        if (year < kMinYear || year > kMaxYear) {
            throw std::out_of_range("年份超出支持范围（1~3000）");
        }
    }

    void validateMonth(int month) const {
        if (month < 1 || month > 12) {
            throw std::out_of_range("月份非法（应为1~12）");
        }
    }

    void validateDate(int year, int month, int day) const {
        validateYear(year);
        validateMonth(month);
        if (day < 1 || day > daysInMonth(year, month)) {
            throw std::out_of_range("日期非法（超出当月天数）");
        }
    }

    // 从公元 1 年 1 月 1 日到当前年份开始前，一共多少天
    long long daysBeforeYear(int year) const {
        long long y = year - 1;
        return y * 365 + y / 4 - y / 100 + y / 400;
    }

    // 当年从 1 月 1 日到当前月份开始前，一共多少天
    int daysBeforeMonth(int year, int month) const {
        static constexpr std::array<int, 12> kPrefix = {
            0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
        };
        int days = kPrefix[month - 1];
        if (month > 2 && isLeapYear(year)) {
            days += 1;
        }
        return days;
    }
};

std::string weekdayName(int weekday) {
    static constexpr std::array<const char*, 7> kNames = {
        "周一", "周二", "周三", "周四", "周五", "周六", "周日"
    };
    return kNames[weekday];
}

void printMenu() {
    std::cout << "\n================= 万年历（公元1年~3000年） =================\n";
    std::cout << "1. 查询某一天是星期几\n";
    std::cout << "2. 打印某年某月的月历\n";
    std::cout << "3. 查询某天是当年的第几天\n";
    std::cout << "0. 退出\n";
    std::cout << "请选择功能：";
}

int main() {
    PerpetualCalendar calendar;

    while (true) {
        printMenu();
        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cerr << "输入格式错误，程序结束。\n";
            return 1;
        }

        if (choice == 0) {
            std::cout << "感谢使用，再见！\n";
            break;
        }

        try {
            if (choice == 1) {
                int y, m, d;
                std::cout << "请输入日期（年 月 日）：";
                std::cin >> y >> m >> d;
                int w = calendar.dayOfWeek(y, m, d);
                std::cout << y << "-" << m << "-" << d << " 是 " << weekdayName(w) << "\n";
            } else if (choice == 2) {
                int y, m;
                std::cout << "请输入年和月（年 月）：";
                std::cin >> y >> m;
                calendar.printMonthCalendar(y, m);
            } else if (choice == 3) {
                int y, m, d;
                std::cout << "请输入日期（年 月 日）：";
                std::cin >> y >> m >> d;
                int n = calendar.dayOfYear(y, m, d);
                std::cout << y << "-" << m << "-" << d << " 是当年的第 " << n << " 天\n";
            } else {
                std::cout << "无效选项，请重新输入。\n";
            }
        } catch (const std::exception& ex) {
            std::cout << "错误：" << ex.what() << "\n";
        }
    }

    return 0;
}
