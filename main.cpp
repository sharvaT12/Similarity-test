//
//  Project 2
//  Name:- Sharva Thakur
//  UIN:- 654135206
//
//  This project is about similarity test. This asks people a set of
//  random questions from questions.txt and then takes their answers.
//  Then this processes the data from user and then asks the user for
//  a file to be compared with then inputs the data from the file.
//  Then compares and outputs the person who is most similar the user
//  based on the answers the user gives. This continues to compare the
//  data with different files till the user does not quit.
//

#include <set>

#include <vector>

#include <map>

#include <fstream>

#include <sstream>

#include <string>

#include <iostream>

#include "driver.h"

using namespace std;

//
//  storevalue
//
//  The parameter is stringstream passed by refrence.
//
//  This function stores the data from file into a map<char,int> and
//  returns the map<char,int>. This is used both to store factors in
//  Question and scores in Person.
//
map < char, int > storevalue(stringstream & parser) {
    map < char, int > map;
    char character;
    string f;

    while (!parser.eof()) {
        parser >> character;
        parser >> f;

        f = f.substr(1);
        int num = stoi(f);

        map.emplace(character, num);
    }

    return map;
}

//
//  storeques
//
//  The parameter is a set<Question> passed by refrence.
//
//  This function opens the file questions.txt and then inputs all the
//  content of questions.txt and stores the data into a set of
//  Questions and skips the blank line in the end of file. This
//  function calls storevalue to store the factors
//
void storeques(set < Question > & ques_set) {
    ifstream infile("questions.txt");
    if (!infile.good()) {  //  if file not good
        cout << "Error: unable to open 'questions.txt'" << endl;
        return;
    }
    while (!infile.eof()) {
        Question S;
        string line;

        getline(infile, line);
        stringstream parser;
        if (!line.empty()) {  // to skip the last line
            parser.str(line);

            string question;
            getline(parser, question, '.');

            S.questionText = question;

            S.factors = storevalue(parser);

            ques_set.insert(S);
        }
    }
}

//
//  take_answer
//
//  The parameter for this function is Question which is passsed by refrence
//
//  This function prints the question and all the options. Then takes
//  user's input and return it.
//
int take_answer(Question & N) {
    int answer_num = 0;
    cout << '"' << N.questionText << '.' << '"' << endl;
    cout << endl;
    cout << "1. Strongly disagree" << endl;
    cout << "2. Disagree" << endl;
    cout << "3. Neutral" << endl;
    cout << "4. Agree" << endl;
    cout << "5. Strongly agree" << endl;
    cout << endl;

    cout << "Enter your answer here (1-5): " << endl;
    cin >> answer_num;  //  takes user's input
    return answer_num;
}

//
//  ask_question
//
//  The parameters are int, set of Question and map<Question, int>
//  passed by refrence.
//
//  This function takes a random question from the set of Question by
//  calling the randomQuestionFrom function in driver.h. Then takes
//  users answer and store it into a map of <Question,int>. Then this
//  calls scoresFrom function in driver.h and returns the
//  map<char,int> with the user's score.
//
map < char, int > ask_question(int & num_ques, set < Question > & ques_set,
    map < Question, int > & user_ans) {

    storeques(ques_set);

    for (int i = 0; i < num_ques; i++) {
        //  calling randomQuestionFrom from driver.h.
        Question N = randomQuestionFrom(ques_set);
        cout << "How much do you agree with this statement?" << endl;
        int s = take_answer(N);
        user_ans.emplace(N, s);
    }

    //  calling scoresFrom from driver.h
    map < char, int > ans_scores = scoresFrom(user_ans);
    return ans_scores;
}

//
//  user_file
//
//  This function prints all the available files and takes the user's
//  answer so that it can process that file. This function returns the
//  number of the file in int.
//
int user_file() {
    cout << "1. BabyAnimals" << endl;
    cout << "2. Brooklyn99" << endl;
    cout << "3. Disney" << endl;
    cout << "4. Hogwarts" << endl;
    cout << "5. MyersBriggs" << endl;
    cout << "6. SesameStreet" << endl;
    cout << "7. StarWars" << endl;
    cout << "8. Vegetables" << endl;
    cout << "9. mine" << endl;
    cout << "0. To end program." << endl;

    cout << endl;

    cout << "Choose test number (1-9, or 0 to end): ";

    int n = 0;
    cin >> n;

    return n;
}

//
//  filename
//
//  The parameter is int passed by refrence.
//
//  This function returns the name of file which the user chooses.
//
string filename(int & file_num) {
    string file;
    if (file_num == 1) {
        file = "BabyAnimals.people";
    } else if (file_num == 2) {
        file = "Brooklyn99.people";
    } else if (file_num == 3) {
        file = "Disney.people";
    } else if (file_num == 4) {
        file = "Hogwarts.people";
    } else if (file_num == 5) {
        file = "MyersBriggs.people";
    } else if (file_num == 6) {
        file = "SesameStreet.people";
    } else if (file_num == 7) {
        file = "StarWars.people";
    } else if (file_num == 8) {
        file = "Vegetables.people";
    } else if (file_num == 9) {
        file = "mine.people";
    }
    return file;
}

//
//  input_people
//
//  The parameters are set of Person and int which are passed by refrence.
//
//  This function opens the file the user chooses and takes the input
//  but does not take the last black line in the file. Then stores the
//  data into the set of Person which is passed in the parameter.
//
void input_people(set < Person > & people, int & file_num) {
    people.clear();
    string A = filename(file_num);
    ifstream infile(A);
    if (!infile.good()) {  //  if file not good
        cout << "Error: unable to open '" << A << "'" << endl;
        return;
    }
    while (!infile.eof()) {
        Person S;
        string line;

        getline(infile, line);
        stringstream parser;
        if (!line.empty()) {  //  if the line is empty.
            parser.str(line);

            string name;
            getline(parser, name, '.');

            S.name = name;

            S.scores = storevalue(parser);

            people.insert(S);
        }
    }
}

//
//  prosfile
//
//  The parameters are set of Person and map<char, int> which is the
//  users scores both passed by refrence.
//  
//  This function processes and finds the similar person untill the
//  user decides to quit. This function calles mostSimilarTo from
//  driver.h to get the person which is most similar to the user and
//  then prints the name of the person from data.
//
void prosfile(set < Person > & people, map < char, int > & user_score) {
    int file_num = user_file();

    while (file_num != 0) {
        input_people(people, file_num);

        Person sim;
        //  calls mostSimilarTo
        sim = mostSimilarTo(user_score, people);

        cout << "You got " << sim.name << "!" << endl;
        cout << endl;
        file_num = user_file();
    }

    cout << "Goodbye!" << endl;
}

//
//  main
//
int main() {
    set < Question > ques_set;
    map < Question, int > user_ans;
    map < char, int > user_score;
    int num_ques = 0;
    set < Person > people;
    cout << "Welcome to the Personality Quiz!" << endl;
    cout << endl;

    cout << "Choose number of questions: " << endl;

    cin >> num_ques;  //  number of questions

    user_score = ask_question(num_ques, ques_set, user_ans);

    prosfile(people, user_score);

    return 0;
}
