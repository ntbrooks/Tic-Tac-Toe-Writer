#include <fstream>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

// hard-coded text, for testing purposes only
vector<string> sections = {
    "\nLorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor.\n",
    "Sed ut perspiciatis unde omnis iste natus error sit voluptatem.\n",
    "But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain.\n",
    "At vero eos et accusamus et iusto odio.\n",
    "On the other hand, we denounce with righteous indignation and dislike men who are so beguiled.\n"};
const string FILE_NAME = "../TicTacToeWriter/writer.txt";
const int FILE_TYPE  = 164;

struct Header {
    uint32_t fileType;
    uint32_t indexSize;
    uint32_t movesStart;
    uint32_t movesLength;
    uint32_t playerXNameStart;
    uint32_t playerXNameLength;
    uint32_t playerONameStart;
    uint32_t playerONameLength;
};

struct Record {
    uint32_t location;
    uint32_t length;
};

void WriteHeader(Header &header, ofstream &file) {
    file.seekp(0);
    file.write((char *)&header, sizeof(header));
}

void WriteRecord(int index, Record &record, ofstream &file) {
    file.seekp(sizeof(Header) + index * sizeof(record));
    file.write((char *)&record, sizeof(record));
}

int WriteSection(int start, int &text, ofstream &file) {
    file.seekp(start);
    file << text;
    return (int)file.tellp() - start;
}

int main(int argc, char *argv[]) {
    ofstream file;
    Header header;
    Record record;
    uint32_t nextLocation;
    int bytesWritten;
    string playerX, playerY;

    file.open(FILE_NAME, ios::out | ios::binary);
    header.fileType = FILE_TYPE;
    header.indexSize = sections.size();
    WriteHeader(header, file);

    cout << "Enter a name for player X: ";
    cin >> playerX;

    cout << "Enter a name for player Y: ";
    cin >> playerY;

    char mark[] = {'X','O'};
    char board[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    int move = 0;
    vector<int> playerMoves;
    string winner = "";

    for (int turn = 0; turn < 9; turn++) {
        //draw the board
        cout << " " << board[0] << " | " << board[1] << " | " << board[2] << endl;
        cout << " -----------" << endl;
        cout << " " << board[3] << " | " << board[4] << " | " << board[5] << endl;
        cout << " -----------" << endl;
        cout << " " << board[6] << " | " << board[7] << " | " << board[8] << endl;

        //get next move
        cout << endl << "Player " << (turn%2)+1 << " select cell (1 - 9)" << endl;
        cin >> move;

        if(board[move-1] != ' ') {
            cout << "That cell is taken! You lose" << endl;
            return 0;
        }

        board[move-1] = mark[turn%2];
        playerMoves.push_back(move);

        if (board[0] == mark[0] && board[1] == mark[0] && board[2] == mark[0]) {
            winner = "Player 1 Wins";
            break;
        } else if (board[0] == mark[1] && board[1] == mark[1] && board[2] == mark[1]) {
            winner = "Player 2 Wins";
            break;
        } else if (board[3] == mark[0] && board[4] == mark[0] && board[5] == mark[0]) {
            winner = "Player 1 Wins";
            break;
        } else if (board[3] == mark[1] && board[4] == mark[1] && board[5] == mark[1]) {
            winner = "Player 2 Wins";
            break;
        } else if (board[6] == mark[0] && board[7] == mark[0] && board[8] == mark[0]) {
            winner = "Player 1 Wins";
            break;
        } else if (board[6] == mark[1] && board[7] == mark[1] && board[8] == mark[1]) {
            winner = "Player 2 Wins";
            break;
        } else if (board[0] == mark[0] && board[4] == mark[0] && board[8] == mark[0]) {
            winner = "Player 1 Wins";
            break;
        } else if (board[0] == mark[1] && board[4] == mark[1] && board[8] == mark[1]) {
            winner = "Player 2 Wins";
            break;
        } else if (board[2] == mark[0] && board[4] == mark[0] && board[6] == mark[0]) {
            winner = "Player 1 Wins";
            break;
        } else if (board[2] == mark[0] && board[4] == mark[0] && board[6] == mark[0]) {
            winner = "Player 2 Wins";
            break;
        } else {
            winner = "Cat's Game!";
        }
    }

    nextLocation = sizeof(Header) + sections.size() * sizeof(Record);

    for(int i = 0; i < sections.size(); i++) {
        bytesWritten = WriteSection(nextLocation, playerMoves[i], file);
        record.location = nextLocation;
        record.length = bytesWritten;
        nextLocation = file.tellp();
        WriteRecord(i, record, file);
        file << endl;
    }

    file << "PlayerX: " << playerX << endl;
    file << "PlayerY: " << playerY << endl;
    for(int i : playerMoves) {
        file << i << endl;
    }

    cout << winner << endl;
    cout << "Game Over" << endl;
    file.close();

    return 0;
}
