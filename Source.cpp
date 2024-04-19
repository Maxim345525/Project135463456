#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cctype>
using namespace std;

// Функція для розшифрування рядка (модель шифрування/розшифрування)
string decrypt(const string& encryptedText) {
    string decryptedText;
    for (char c : encryptedText) {
        // В даному випадку застосовуємо простий зсув на 1 символ для розшифрування (Caesar cipher)
        decryptedText += char(c - 1);
    }
    return decryptedText;
}
// Функція для зчитування слів з файлу
vector<string> readWordsFromFile(const string& filename) {
    ifstream file(filename);
    vector<string> words;
    if (!file) {
        cerr << "Could not open word file.\n";
        return words;
    }

    string encryptedWord;
    while (getline(file, encryptedWord)) {
        // Розшифрувати слово та додати до списку
        string word = decrypt(encryptedWord);
        words.push_back(word);
    }

    file.close();
    return words;
}

// Функція для вибору випадкового слова зі списку
string chooseRandomWord(const vector<string>& words) {
    srand(time(0));
    int randomIndex = rand() % words.size();
    return words[randomIndex];
}

// Функція для оновлення поточного стану слова
string updateWordState(const string& word, const string& guessedLetters) {
    string state;
    for (char c : word) {
        // Якщо літера вгадана, показати її, інакше - '_'
        if (guessedLetters.find(c) != string::npos) {
            state += c;
        }
        else {
            state += '_';
        }
    }
    return state;
}

// Функція для перевірки, чи гравець виграв гру
bool isWordComplete(const string& wordState) {
    return wordState.find('_') == string::npos;
}

int main() {
    // Зчитати слова з файлу
    const string filename = "words.txt";
    vector<string> words = readWordsFromFile(filename);
    if (words.empty()) {
        cerr << "Word list is empty or file not found.\n";
        return 1;
    }

    // Вибрати випадкове слово для гри
    string word = chooseRandomWord(words);

    // Ініціалізація змінних гри
    string guessedLetters;
    string currentState = updateWordState(word, guessedLetters);
    int maxAttempts = 7; // Максимальна кількість спроб
    int attemptsLeft = maxAttempts;

    auto startTime = chrono::system_clock::now(); // Початок гри

    // Головний цикл гри
    while (attemptsLeft > 0) {
        cout << "The current state of the word: " << currentState << endl;
        cout << "Remaining attempts: " << attemptsLeft << endl;
        cout << "Enter a letter: ";
        char guessedLetter;
        cin >> guessedLetter;

        // Перевірка, чи літера вже була вгадана
        if (guessedLetters.find(guessedLetter) != string::npos) {
            cout << "The letter has already been guessed, try another one.\n";
            continue;
        }

        // Додати вгадану літеру до списку вгаданих
        guessedLetters += guessedLetter;

        // Оновити поточний стан слова
        currentState = updateWordState(word, guessedLetters);

        // Перевірити, чи гравець виграв
        if (isWordComplete(currentState)) {
            cout << "\nYou guessed the word! The word was: " << word << endl;
            break;
        }

        // Зменшити кількість спроб, якщо літера не вгадана
        if (word.find(guessedLetter) == string::npos) {
            attemptsLeft--;
            cout << "Літера не вгадана, залишилось спроб: " << attemptsLeft << endl;
        }
    }

    // Завершення гри
    auto endTime = chrono::system_clock::now(); // Кінець гри
    chrono::duration<double> duration = endTime - startTime;
    double elapsedTime = duration.count();

    // Вивід статистики гри
    cout << "\nGame over.\n";
    if (attemptsLeft == 0) {
        cout << "You lose.Word is: " << word << endl;
    }
    cout << "Game time: " << elapsedTime << " seconds." << endl;
    cout << "Attempts have been made: " << maxAttempts - attemptsLeft << endl;
    cout << "Used letters: " << guessedLetters << endl;

    return 0;
}
