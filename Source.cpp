#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cctype>
using namespace std;

// ������� ��� ������������� ����� (������ ����������/�������������)
string decrypt(const string& encryptedText) {
    string decryptedText;
    for (char c : encryptedText) {
        // � ������ ������� ����������� ������� ���� �� 1 ������ ��� ������������� (Caesar cipher)
        decryptedText += char(c - 1);
    }
    return decryptedText;
}
// ������� ��� ���������� ��� � �����
vector<string> readWordsFromFile(const string& filename) {
    ifstream file(filename);
    vector<string> words;
    if (!file) {
        cerr << "Could not open word file.\n";
        return words;
    }

    string encryptedWord;
    while (getline(file, encryptedWord)) {
        // ������������ ����� �� ������ �� ������
        string word = decrypt(encryptedWord);
        words.push_back(word);
    }

    file.close();
    return words;
}

// ������� ��� ������ ����������� ����� � ������
string chooseRandomWord(const vector<string>& words) {
    srand(time(0));
    int randomIndex = rand() % words.size();
    return words[randomIndex];
}

// ������� ��� ��������� ��������� ����� �����
string updateWordState(const string& word, const string& guessedLetters) {
    string state;
    for (char c : word) {
        // ���� ����� �������, �������� ��, ������ - '_'
        if (guessedLetters.find(c) != string::npos) {
            state += c;
        }
        else {
            state += '_';
        }
    }
    return state;
}

// ������� ��� ��������, �� ������� ������ ���
bool isWordComplete(const string& wordState) {
    return wordState.find('_') == string::npos;
}

int main() {
    // ������� ����� � �����
    const string filename = "words.txt";
    vector<string> words = readWordsFromFile(filename);
    if (words.empty()) {
        cerr << "Word list is empty or file not found.\n";
        return 1;
    }

    // ������� ��������� ����� ��� ���
    string word = chooseRandomWord(words);

    // ����������� ������ ���
    string guessedLetters;
    string currentState = updateWordState(word, guessedLetters);
    int maxAttempts = 7; // ����������� ������� �����
    int attemptsLeft = maxAttempts;

    auto startTime = chrono::system_clock::now(); // ������� ���

    // �������� ���� ���
    while (attemptsLeft > 0) {
        cout << "The current state of the word: " << currentState << endl;
        cout << "Remaining attempts: " << attemptsLeft << endl;
        cout << "Enter a letter: ";
        char guessedLetter;
        cin >> guessedLetter;

        // ��������, �� ����� ��� ���� �������
        if (guessedLetters.find(guessedLetter) != string::npos) {
            cout << "The letter has already been guessed, try another one.\n";
            continue;
        }

        // ������ ������� ����� �� ������ ��������
        guessedLetters += guessedLetter;

        // ������� �������� ���� �����
        currentState = updateWordState(word, guessedLetters);

        // ���������, �� ������� ������
        if (isWordComplete(currentState)) {
            cout << "\nYou guessed the word! The word was: " << word << endl;
            break;
        }

        // �������� ������� �����, ���� ����� �� �������
        if (word.find(guessedLetter) == string::npos) {
            attemptsLeft--;
            cout << "˳���� �� �������, ���������� �����: " << attemptsLeft << endl;
        }
    }

    // ���������� ���
    auto endTime = chrono::system_clock::now(); // ʳ���� ���
    chrono::duration<double> duration = endTime - startTime;
    double elapsedTime = duration.count();

    // ���� ���������� ���
    cout << "\nGame over.\n";
    if (attemptsLeft == 0) {
        cout << "You lose.Word is: " << word << endl;
    }
    cout << "Game time: " << elapsedTime << " seconds." << endl;
    cout << "Attempts have been made: " << maxAttempts - attemptsLeft << endl;
    cout << "Used letters: " << guessedLetters << endl;

    return 0;
}
