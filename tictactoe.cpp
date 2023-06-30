/*
Tic Tac Toe Player

NOTE: This code is written for practice. Consequently, the code is:
- more complex than it should be
- not always optimised
- willingly not using better alternatives provided by the standard library

I will make a new version of this following best practices.
*/

#include <iostream>
#include <iterator> // for std::size
#include <limits>
#include <vector>

constexpr short N{ 3 }; // grid size (only 3 works currently)
constexpr char X{ 'X' };
constexpr char O{ 'O' };

struct Point
{
    int x{};
    int y{};
};

struct Pair
{
    Point action{};
    int utility{};
};

char get_user()
{   
    while (true)
    {
        std::cout << "Pick a symbol (X/O): ";
        char user{};
        std::cin >> user;

        // check for failed instruction
        if (!std::cin)
        {   
            if (std::cin.eof()) // if the stream was closed
            {
                exit(0); // exit the program
            }
            // handle the failure
            std::cin.clear(); // return to 'normal' mode
        }

        // remove extraneous input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // validate input
        if (user == X || user == O) return user;
        std::cout << "Invalid input, please try again.\n";
    }
}

char** initial_state()
{   
    /*
    Returns starting state of the board (dynamic two-dimensional array).
    Each row is represented by an array of chars.
    Returns a pointer to an array of pointers to each row.
    */

    // create a pointer to an array of N pointers
    char** array{ new char*[N] };

    // point each of these pointers to a row of the grid
    for (int i{ 0 }; i < N; ++i)
        array[i] = new char[N]{};
    
    // return pointer
    return array;
}

void print_state(char** state)
{   
    /*
    Outputs the board in the terminal.
    */
    for (int i{ 0 }; i < N; ++i)
    {   
        // print row of cells
        std::cout << ' '; 
        for (int j{ 0 }; j < N; ++j)
            // add space if cell is empty, add newline after the last cell, else |.
            std::cout << state[i][j] << (state[i][j] ? "" : " ") << (j == N-1 ? "\n" : " | ");

        if (i == N-1) break; // do not print horizontal line for the last row

        // print horizontal line
        for (int k{ 0 }; k < N-1; ++k)
            std::cout << "---+";
        std::cout << "---\n";
    }
    std::cout << '\n';
}

char get_winner(char** state)
{
    /*
    Returns a winner, if there is one.
    */

    // Check rows.
    for (int i{ 0 }; i < N; ++i)
    {   
        bool flag{ true };

        for (int j{ 0 }; j < N-1; ++j)
        {
            if (state[i][j] != state[i][j+1])
            {
                flag = false;
                break;
            }
        }

        if (flag) return state[i][0];
    }

    // Check columns.
    for (int i{ 0 }; i < N; ++i)
    {   
        bool flag{ true };

        for (int j{ 0 }; j < N-1; ++j)
        {
            if (state[j][i] != state[j+1][i])
            {
                flag = false;
                break;
            }
        }

        if (flag) return state[0][i];
    }

    // Check diagonals.
    bool flag{ true };
    for (int i{ 0 }; i < N-1; ++i)
    {
        if (state[i][i] != state[i+1][i+1])
        {
            flag = false;
            break;
        }
    }
    if (flag) return state[0][0];


    flag = true;
    for (int i{ 0 }; i < N-1; ++i)
    {
        if (state[i][N-1-i] != state[i+1][N-1-(i+1)])
        {
            flag = false;
            break;
        }
    }
    if (flag) return state[0][N-1];

    return 'F';
}

std::vector<Point> get_actions(char** state)
{
    /*
    Returns vector of all possible actions Point(x, y) available in 'state'.
    */

    int count{ 0 };

    for (int i{ 0 }; i < N; ++i)
    {
        for (int j{ 0 }; j < N; ++j)
        {
            if (!state[i][j])
                ++count;
        }
    }

    std::vector<Point> actions(count);

    int index{ 0 };
    for (int i{ 0 }; i < N; ++i)
    {
        for (int j{ 0 }; j < N; ++j)
        {
            if (!state[i][j])
            {
                actions[index] = Point{ i, j };
                ++index;
            }
        }
    }
    return actions;
}

bool terminal(char** state)
{
    /*
    Returns true if game is over (someone won or all cells are filled), false otherwise.
    */
   char winner{ get_winner(state) };
   std::vector<Point> actions{ get_actions(state) };
   return (winner == X || winner == O || !actions.size());
}

char get_player(char** state)
{   
    /*
    Returns player who has the next turn.
    It is O's turn if the number of empty cells is even, else X. 
    */
    int count{};
    for (int i{ 0 }; i < N; ++i)
    {
        for (int j{ 0 }; j < N; ++j)
        {
            if (!state[i][j])
                ++count;
        }
    }
    return (count % 2 == 0 ? O: X);
}

char** result(char** state, Point move)
{
    /*
    Returns the board that results from making move (i, j)
    */
   
    // create an empty board and copy values into the new one
    char** new_state{ initial_state() };

    for (int i{ 0 }; i < N; ++i)
    {
        for (int j{ 0 }; j < N; ++j)
        {   
            if (i == move.x && j == move.y)
                new_state[i][j] = get_player(state);
            else
                new_state[i][j] = state[i][j];
        }
    }

    return new_state;
}

int utility(char** state)
{
    /*
    Returns 1 if X has won the game, -1 if O has won, 0 otherwise.
    */
    switch( get_winner(state) )
    {
        case X: return 1;
        case O: return -1;
        default: return 0;
    }
}

Point get_user_move(char** state)
{
    while (true)
    {
        std::cout << "Enter x-coordinate: ";
        int x{};
        std::cin >> x;
        std::cout << "Enter y-coordinate: ";
        int y{};
        std::cin >> y;

        // check for failed instruction
        if (!std::cin)
        {   
            if (std::cin.eof()) // if the stream was closed
            {
                exit(0); // exit the program
            }
            // handle the failure
            std::cin.clear(); // return to 'normal' mode
        }

        // remove extraneous input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Check if move is in bounds and still possible.
        if (x >= 0 && x < N && y >= 0 && y < N)
        {   
            Point move{ x, y };
            std::vector<Point> actions{ get_actions(state) };
            for (Point point : actions)
            {
                if (point.x == move.x && point.y == move.y)
                    return move;
            }
        };
        std::cout << "Invalid input, please try again.\n";
    }
}

int max_value(char** state);

int min_value(char** state)
{
    if (terminal(state))
        return utility(state);
    
    // Return the lowest utility, given optimal play by the maximising player.
    int v{ 10 };

    for ( Point action: get_actions(state) )
    {   
        int max_utility{ max_value(result(state, action))};
        if (max_utility < v)
            v = max_utility;
    }
    return v;
}

int max_value(char** state)
{
    if (terminal(state))
        return utility(state);
    
    // Return the highest utility, given optimal play by the minimising player.
    int v{ -10 };

    for ( Point action: get_actions(state) )
    {   
        int min_utility{ min_value(result(state, action))};
        if (min_utility > v)
            v = min_utility;
    }
    return v;
}

Point minimax(char** state)
{   
    // Calculate each move's utility and store them as utility-move pair in dictionary.
    // Future feature: add alpha-beta pruning.

    std::vector<Point> actions{ get_actions(state) };
    std::vector<Pair> moves(actions.size());
    char player{ get_player(state) };

    for (int i{ 0 }; Point action : actions )
    {
        if ( player == X )
        {
            moves[i] = Pair{ action, min_value(result(state, action))};
        }
        else
        {
            moves[i] = Pair{ action, max_value(result(state, action))};
        }
        ++i;
    }

    // Return move with the best utility
    if (player == X)
    {   
        // Pick move with highest utility.
        Pair highest{ moves[0] };
        for (Pair move: moves)
        {
            if (move.utility > highest.utility)
                highest = move;
        }
        return highest.action;
    }
    else
    {
        // Pick move with lowest utility.
        Pair lowest{ moves[0] };
        for (Pair move: moves)
        {
            if (move.utility < lowest.utility)
                lowest = move;
        }
        return lowest.action;
    }
}

void delete_state(char** state)
{
    for (int i{ 0 }; i < N; ++i)
    {
        delete[] state[i];
    }
    delete[] state;
}

int main()
{   
    // Let user pick a symbol (X|O and X begins). 
    char user{ get_user() };

    // Create a new board.
    char** board { initial_state() };
    print_state(board);

    // Main game loop.
    while (true)
    {   
        // Check game status and whose turn it is.
        bool game_over{ terminal(board) };
        char player{ get_player(board) };

        if (game_over)
        {   
            char winner{ get_winner(board) };
            switch( winner )
            {
                case X:
                case O:
                    std::cout << "Game over: " << winner << " wins.\n";
                    break;
                default:
                    std::cout << "Game over: Tie.\n";
            }
            break;
        }

        // Get AI move.
        if (user != player)
        {
            Point move{ minimax(board) };
            board = result(board, move);
            print_state(board);
            continue;
        }
        
        // Get user move.
        Point move{ get_user_move(board) };
        board = result(board, move);
        print_state(board);
    }

    // Free up dynamically created memory (what about all the copies?)
    delete_state(board);
    return 0;
}
