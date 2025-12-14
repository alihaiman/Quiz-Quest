/*
=========================================
Quiz System – C++ Project
-----------------------------------------
Group Members:
1. Aliha Iman         (25F-0567)
2. Naima Maitla       (25F-0590)
3. Syeda Kashaf Zahra (25F-0663)

Description:
Console-based quiz system supporting
multiple categories, difficulty levels,
leaderboard management, session logging,
and review of incorrect answers.

Environment:
Microsoft Visual Studio (C++)
=========================================
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
Purpose:
Stores a single quiz question along with
its difficulty level, options, and correct answer.
*/
struct Question {
	int difficulty;
	string text;
	string optA;
	string optB;
	string optC;
	string optD;
	int correctOption;
};

/*
Purpose:
Stores leaderboard data including player
name, date, score, and difficulty level.
*/
struct LeaderboardEntry {
	string name;
	string date;
	int score;
	int difficulty;
};

/*
Function Prototypes
*/
void mainMenu();
void startQuiz();
int loadQuestions(Question qArray[], int maxQ, string filename, int diff);
void showQuestion(Question q);
int getAnswer();
void showLeaderboard();
void saveLeaderboard(string name, int score, int diff);
void logSession(string name, int correct, int incorrect, int score);
void reviewWrong(Question wrongQ[], int wrongAns[], int wrongCount);

/*
Purpose:
Program entry point. Starts the main menu.
*/
int main() {
	mainMenu();
	system("pause");
	return 0;
}

/*
Purpose:
Displays the main menu and handles user
navigation between quiz, leaderboard, and exit.
*/
void mainMenu() {
	int choice;

	while (true) {
		cout << "==========================" << endl;
		cout << "       QUIZ SYSTEM        " << endl;
		cout << "==========================" << endl;
		cout << "1. Start New Quiz" << endl;
		cout << "2. View Leaderboard" << endl;
		cout << "3. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, ' ');
			cout << "Invalid input." << endl;
		}
		else if (choice == 1) {
			startQuiz();
		}
		else if (choice == 2) {
			showLeaderboard();
		}
		else if (choice == 3) {
			break;
		}
		else {
			cout << "Invalid choice." << endl;
		}

		cout << endl;
	}
}

/*
Purpose:
Loads quiz questions from a file based on
selected difficulty level.

Parameters:
qArray   - Array to store questions
maxQ     - Maximum questions allowed
filename - File containing questions
diff     - Difficulty level

Return:
Number of questions loaded
*/
int loadQuestions(Question qArray[], int maxQ, string filename, int diff) {
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
		cout << "Could not open file." << endl;
		return 0;
	}

	int count = 0;
	while (count < maxQ) {
		Question q;
		if (!(fin >> q.difficulty)) {
			break;
		}
		fin.ignore();
		getline(fin, q.text);
		getline(fin, q.optA);
		getline(fin, q.optB);
		getline(fin, q.optC);
		getline(fin, q.optD);
		fin >> q.correctOption;
		fin.ignore();

		if (q.difficulty == diff) {
			qArray[count] = q;
			count++;
		}
	}

	fin.close();
	return count;
}

/*
Purpose:
Displays a single quiz question and its options.
*/
void showQuestion(Question q) {
	cout << endl;
	cout << q.text << endl;
	cout << "1) " << q.optA << endl;
	cout << "2) " << q.optB << endl;
	cout << "3) " << q.optC << endl;
	cout << "4) " << q.optD << endl;
}

/*
Purpose:
Takes and validates user's answer input.

Return:
Valid option number (1–4)
*/
int getAnswer() {
	int ans;
	cout << "Enter your answer (1-4): ";
	cin >> ans;
	while (cin.fail() || ans < 1 || ans > 4) {
		cin.clear();
		cin.ignore(1000, ' ');
		cout << "Enter a valid option (1-4): ";
		cin >> ans;
	}
	return ans;
}

/*
Purpose:
Manages the complete quiz process including
category selection, scoring, and review.
*/
void startQuiz() {
	int category;
	int diff;

	cout << endl;
	cout << "Select Category:" << endl;
	cout << "1. Science" << endl;
	cout << "2. Computer" << endl;
	cout << "3. Sports" << endl;
	cout << "4. IQ" << endl;
	cout << "Enter: ";
	cin >> category;

	string file;

	if (category == 1)
		file = "science.txt";
	else if (category == 2)
		file = "computer.txt";
	else if (category == 3)
		file = "sports.txt";
	else if (category == 4)
		file = "iq.txt";
	else {
		cout << "Invalid category." << endl;
		return;
	}

	cout << "Difficulty (1=Easy, 2=Medium, 3=Hard): ";
	cin >> diff;

	const int MAX_Q = 100;
	Question allQ[MAX_Q];
	int totalQ = loadQuestions(allQ, MAX_Q, file, diff);

	if (totalQ < 10) {
		cout << "Not enough questions for this difficulty." << endl;
		return;
	}

	int usedIndex[10];
	for (int i = 0; i < 10; i++) {
		usedIndex[i] = -1;
	}

	Question wrongQ[10];
	int wrongAns[10];
	int wrongCount = 0;

	int score = 0;
	int correct = 0;
	int incorrect = 0;

	srand((unsigned)time(0));

	for (int qNum = 0; qNum < 10; qNum++) {
		int index = rand() % totalQ;

		bool repeat = true;
		while (repeat) {
			repeat = false;
			for (int j = 0; j < qNum; j++) {
				if (usedIndex[j] == index) {
					repeat = true;
					index = rand() % totalQ;
					break;
				}
			}
		}

		usedIndex[qNum] = index;

		Question q = allQ[index];
		cout << endl;
		cout << "Question " << (qNum + 1) << " of 10" << endl;
		showQuestion(q);

		int ans = getAnswer();

		if (ans == q.correctOption) {
			cout << "Correct." << endl;
			correct++;
			score = score + 10;
		}
		else {
			cout << "Wrong." << endl;
			incorrect++;
			if (diff == 1) score = score - 2;
			else if (diff == 2) score = score - 3;
			else score = score - 5;

			wrongQ[wrongCount] = q;
			wrongAns[wrongCount] = ans;
			wrongCount++;
		}
	}

	cout << endl;
	cout << "=========== QUIZ COMPLETE ===========" << endl;
	cout << "Correct: " << correct << endl;
	cout << "Incorrect: " << incorrect << endl;
	cout << "Final Score: " << score << endl;

	string name;
	cout << "Enter your name: ";
	cin >> name;

	saveLeaderboard(name, score, diff);
	logSession(name, correct, incorrect, score);

	char c;
	cout << "Do you want to review wrong answers? (Y/N): ";
	cin >> c;
	if (c == 'Y' || c == 'y') {
		reviewWrong(wrongQ, wrongAns, wrongCount);
	}
}

/*
Purpose:
Displays the leaderboard from file.
*/
void showLeaderboard() {
	ifstream fin("high_scores.txt");
	if (!fin.is_open()) {
		cout << "No leaderboard file yet." << endl;
		return;
	}

	cout << endl;
	cout << "========== LEADERBOARD ==========" << endl;

	LeaderboardEntry e;
	while (fin >> e.name >> e.date >> e.score >> e.difficulty) {
		cout << "Name: " << e.name
			<< "  Date: " << e.date
			<< "  Score: " << e.score
			<< "  Diff: " << e.difficulty << endl;
	}

	fin.close();
}

/*
Purpose:
Saves top scores to leaderboard file
and keeps only top 5 entries.
*/
void saveLeaderboard(string name, int score, int diff) {
	LeaderboardEntry arr[20];
	int count = 0;

	ifstream fin("high_scores.txt");
	if (fin.is_open()) {
		while (count < 20 && fin >> arr[count].name >> arr[count].date >> arr[count].score >> arr[count].difficulty) {
			count++;
		}
		fin.close();
	}

	time_t now = time(0);
	tm t;
	localtime_s(&t, &now);

	string date = to_string(t.tm_mday);
	date += "-";
	date += to_string(t.tm_mon + 1);
	date += "-";
	date += to_string(t.tm_year + 1900);

	arr[count].name = name;
	arr[count].date = date;
	arr[count].score = score;
	arr[count].difficulty = diff;
	count++;

	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			if (arr[j].score > arr[i].score) {
				LeaderboardEntry temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}

	if (count > 5) {
		count = 5;
	}

	ofstream fout("high_scores.txt");
	for (int i = 0; i < count; i++) {
		fout << arr[i].name << " " << arr[i].date << " " << arr[i].score << " " << arr[i].difficulty << endl;
	}
	fout.close();
}

/*
Purpose:
Logs quiz session details to a file.
*/
void logSession(string name, int correct, int incorrect, int score) {
	ofstream fout("quiz_logs.txt", ios::app);

	time_t now = time(0);
	char buff[30];
	ctime_s(buff, sizeof(buff), &now);

	fout << "============================" << endl;
	fout << "Player: " << name << endl;
	fout << "Date: " << buff;
	fout << "Correct: " << correct << endl;
	fout << "Incorrect: " << incorrect << endl;
	fout << "Final Score: " << score << endl;
	fout << "============================" << endl << endl;

	fout.close();
}

/*
Purpose:
Displays all incorrectly answered questions
for user review.
*/
void reviewWrong(Question wrongQ[], int wrongAns[], int wrongCount) {
	if (wrongCount == 0) {
		cout << "No wrong answers." << endl;
		return;
	}

	cout << endl;
	cout << "=========== REVIEW MODE ===========" << endl;

	for (int i = 0; i < wrongCount; i++) {
		cout << endl;
		cout << "Question: " << wrongQ[i].text << endl;
		cout << "Your answer: " << wrongAns[i] << endl;
		cout << "Correct option: " << wrongQ[i].correctOption << endl;
	}
}