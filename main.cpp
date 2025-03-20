#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <conio.h>  // Per Windows (kbhit)
#else
    #include <unistd.h>
    #include <sys/select.h>
#endif

using namespace std;

#define MAX_PLAYERS 10
#define MAX_ROUNDS 10
#define MAX_ATTEMPTS 3
#define TIME_LIMIT 30 // Tempo massimo per turno in secondi

const char* phrases[] = {
    "maurizio benazzi, è dal giorno zero che vivi succhiando cazzi",
    "il rosso ti chiede di scoparlo perchè sa che sei ricchione",
    "una severa quanto onesta punizione",
    "il rosso si troverà presto un nuovo amante",
    "uno come il benaz è difficile da trovare"
};
const int num_phrases = sizeof(phrases) / sizeof(phrases[0]);

struct Player {
    char name[50];
    int score;
};

void mask_last_word(const char* phrase, char* masked_phrase) {
    strcpy(masked_phrase, phrase);
    char* last_space = strrchr(masked_phrase, ' ');
    if (last_space) {
        last_space++;
        memset(last_space, '*', strlen(last_space));
    }
}

bool input_available() {
#ifdef _WIN32
    return _kbhit();
#else
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
#endif
}

int main() {
    int num_players, num_rounds;
    Player players[MAX_PLAYERS];

    cout << "Inserisci il numero di giocatori: ";
    cin >> num_players;
    cin.ignore();  // Evita problemi con getline

    for (int i = 0; i < num_players; i++) {
        cout << "Inserisci il nome del giocatore " << i + 1 << ": ";
        cin.getline(players[i].name, 50);
        players[i].score = 0;
    }

    cout << "Inserisci il numero di round: ";
    cin >> num_rounds;
    cin.ignore();

    srand(time(0));

    for (int round = 0; round < num_rounds; round++) {
        for (int i = 0; i < num_players; i++) {
            cout << "\nRound " << round + 1 << " - Turno di " << players[i].name << "\n";

            int phrase_index = rand() % num_phrases;
            char masked_phrase[100];
            mask_last_word(phrases[phrase_index], masked_phrase);

            cout << "Frase: " << masked_phrase << "\n";

            char correct_word[50];
            sscanf(strrchr(phrases[phrase_index], ' ') + 1, "%s", correct_word);

            bool guessed = false;

            for (int attempt = 0; attempt < MAX_ATTEMPTS && !guessed; attempt++) {
                cout << "Tentativo " << attempt + 1 << " - Hai 30 secondi per rispondere!\n";

                time_t start = time(0);
                char user_guess[50] = "";
                bool time_expired = false;

                while (true) {
                    time_t now = time(0);

                    if (now - start >= TIME_LIMIT) {
                        cout << "\n⏳ Tempo scaduto! Nessun punto assegnato.\n";
                        guessed = true;
                        time_expired = true;
                        break;
                    }

                    if (input_available()) {
                        cin.getline(user_guess, 50);
                        if (strcmp(user_guess, correct_word) == 0) {
                            int points = (attempt == 0) ? 10 : (attempt == 1) ? 5 : 3;
                            players[i].score += points;
                            cout << "✅ Corretto! Hai guadagnato " << points << " punti.\n";
                            guessed = true;
                        } else {
                            cout << "❌ Sbagliato!\n";
                            if (attempt == 2) {
                                cout << "La parola era: " << correct_word << " ahahah gay\n";
                            }
                        }
                        break;
                    }

                    this_thread::sleep_for(chrono::milliseconds(100));
                }

                if (time_expired) break;
            }
        }

        cout << "\n🏆 Classifica dopo il round " << round + 1 << ":\n";
        for (int i = 0; i < num_players; i++) {
            cout << players[i].name << ": " << players[i].score << " punti\n";
        }
    }

    Player winner = players[0];
    for (int i = 1; i < num_players; i++) {
        if (players[i].score > winner.score) {
            winner = players[i];
        }
    }

    cout << "\n🏁 Classifica finale:\n";
    for (int i = 0; i < num_players; i++) {
        cout << players[i].name << ": " << players[i].score << " punti\n";
    }

    cout << "\n🎉 Il vincitore è: " << winner.name << " con " << winner.score << " punti!\n";
    cout << "Grazie per aver giocato!";
    return 0;
}