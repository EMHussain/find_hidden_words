#include <iostream>
#include <vector>
#include <cstring>
#include "constants.h"


using idx = std::pair<int, int>; // use to store 2D array x y indexes

void display()
{
    for (size_t i = 0; i < ROW_SIZE; i++)
    {
        for (size_t j = 0; j < COLUMN_SIZE; j++)
        {
            std::cout << puzzle[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void debug(std::vector<idx> matched_idx) // debugging each individual word with postion and case change
{
    std::cout << "word start from { row = " << matched_idx[0].first << ", col = " << matched_idx[0].second << " }" << std::endl;
    std::cout << "word end at { row = " << matched_idx[matched_idx.size() - 1].first << ", col = " << matched_idx[matched_idx.size() - 1].second << " }" << std::endl;
    // std::cout << "To debug further press f or any other key";
    // char c = getline();
    // if(c != 'f')
    char puzzle_copy[ROW_SIZE][COLUMN_SIZE];
    std::copy(&puzzle[0][0], &puzzle[0][0] + ROW_SIZE * COLUMN_SIZE, &puzzle_copy[0][0]); // use copy of array for debugging
    for (auto &item : matched_idx)
    {
        puzzle_copy[item.first][item.second] = toupper(puzzle_copy[item.first][item.second]);
    }
    for (size_t i = 0; i < ROW_SIZE; i++)
    {
        for (size_t j = 0; j < COLUMN_SIZE; j++)
        {
            std::cout << puzzle_copy[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
std::vector<idx> matchWord(const char *word, const size_t &row, const size_t &col)
{
    // clock wise directions from + horizontal (zero degree)
    constexpr int dir_x[8] = {0, 1, 1, -1,  0, -1, -1, -1};
    constexpr int dir_y[8] = {1, 1, 0, 1, -1, -1, 0, 1};

    size_t word_len = strlen(word);
    std::vector<idx> matched_idx;

    for(int i = 0; i < 8; i++)
    {
        std::vector<idx> checked_idx{{row, col}}; // initialize with first idx because it is already matched
        int row_dir = dir_x[i] + row;
        int col_dir = dir_y[i] + col;
        int k;
        for(k = 1; k < word_len; k++)
        {
            if(row_dir >= ROW_SIZE || row_dir < 0 || col_dir >= COLUMN_SIZE || col_dir < 0)
            {
                break; // boundry limit exceed
            }
            if(puzzle[row_dir][col_dir] != word[k])
            {
                break; // mismatch on current direction
            }
            checked_idx.push_back(std::make_pair(row_dir, col_dir)); // save checked idx
            row_dir += dir_x[i];
            col_dir += dir_y[i];
        }

        if(k == word_len)
        {
            if(DEBUG)
            {
                std::cout << "word matched : " << word <<std::endl;
                debug(checked_idx);
                std::cout << "press any to proceed further \n";
                std::cin.get();
            }
            matched_idx = checked_idx; // all values matched keep record later on to change upper case
        }
    }
    return matched_idx;
}

std::vector<idx> findAndReplace(const char * word)
{
    std::vector<idx> matched_idx;
    if(word == NULL)
        return matched_idx;

    for (size_t i = 0; i < ROW_SIZE; i++)
    {
        for (size_t j = 0; j < COLUMN_SIZE; j++)
        {
            if(puzzle[i][j] == word[0]) // match head
            {
                matched_idx = matchWord(word, i, j);
                if(!matched_idx.empty()) // assumed one word accures only once in the matrix
                    return matched_idx;
            }
        }
        
    }
    return matched_idx;
}
void changeToUpperCase(std::vector<std::vector<idx>> all_matched)
{
    for(auto & item : all_matched)
    {
        for (auto &word_idx : item)
        {
            puzzle[word_idx.first][word_idx.second] = toupper(puzzle[word_idx.first][word_idx.second]);
        }
    }
}
int main(int argc, char** argv)
{
    std::cout << "------------------------------befor matched---------------------------------------\n\n";
    display();
    // find all words in matrix
    std::vector<std::vector<idx>> all_matched;
    for(int i = 0; i < WORDS_SIZE; i++)
    {
        std::vector<idx> matched_idx = findAndReplace(words[i]);
        if (!matched_idx.empty()) // check if word exactly matched save indexes to update later
        {
            all_matched.push_back(matched_idx);
        }
        
    }

    // change all words to upper case 
    changeToUpperCase(all_matched);

    std::cout << "\n------------------------------after matched---------------------------------------\n\n";
    display();
    return 0;
}