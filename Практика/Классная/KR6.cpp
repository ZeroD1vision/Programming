#include <iostream>

using namespace std;
int main() {
    const char s[] = "asd+ ";
    int max_count = 0;
    char max_char = 0;

    for (int i = 0; s[i]; i++) {
        // Подсчёт количества s[i]
        int count = 0;
        for (int j = 0; s[j]; j++) {
            if (s[j] == s[i])
                count++;
        }
        // Обновляем максимум, если надо
        if (count > max_count) {
            max_count = count;
            max_char = s[i];
        }
    }

    cout << "Char: '" << max_char << "' Count: " << max_count << '\n';
}