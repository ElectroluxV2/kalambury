// POP 2020-12-07 projekt 1 Budzisz Mateusz EiT 2 184325
// Dev++ Version 5.11
// Visual Studio 2019 Version 16.8.1
// Clion Version 2020.3
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

class Vector {
    unsigned int currentlyAllocated;
    unsigned int count;
    void** ptr;

    void Reallocate(unsigned int newAllocationSize) {
        this->currentlyAllocated = newAllocationSize;
        void** newPtr = (void**)malloc(this->currentlyAllocated * sizeof(void*));
        memcpy(newPtr, this->ptr, this->count * sizeof(void*));
        free(this->ptr);
        this->ptr = newPtr;
    }

public:
    [[nodiscard]] unsigned int Length() const {
        return this->count;
    }

    void Init() {
        this->count = 0;
        this->currentlyAllocated = 1;
        this->ptr = (void**)malloc(this->currentlyAllocated * sizeof(void*));
    }

    void Push(void* val) {
        if (this->count == this->currentlyAllocated) {
            this->Reallocate(2 * this->currentlyAllocated);
        }

        *(this->ptr + this->count) = val;

        this->count++;
    }

    void* Get(unsigned int index) {
        if (index >= this->count) {
            fprintf(stderr, "Vector#Get element index (%i) out of total (%i) count!\n", index, this->count);
            return nullptr;
        }
        return *(this->ptr + index);
    }

    void* Pop() {
        this->count--;
        void* ret = *(this->ptr + this->count);

        if (4 * this->count <= this->currentlyAllocated) {
            this->Reallocate(this->currentlyAllocated / 2);
        }

        return ret;
    }

    void* Begin() {
        return this->ptr;
    }
};

class Game;

class TextUtils {
public:
    enum class Border : int {
        CornerTopLeft = -55,
        CornerTopRight = -69,
        CornerBottomLeft = -56,
        CornerBottomRight = -68,
        Horizontal = -51,
        HorizontalSplitUp = -54,
        HorizontalSplitDown = -53,
        Vertical = -70,
        VerticalSplitRight = -52,
        VerticalSplitLeft = -71,
        Crossing = -50
    };

    enum class Special : int {
        FullArrowRight = 16,
        FullArrowLeft = 17,
        BigDash = 22//-2,
    };

    enum class PolishChars : char {
        c = -122,
        l = -120,
        X = -115,
        C = -113,
        S = -105,
        s = -104,
        L = -99,
        o = -94,
        A = -92,
        a = -91,
        E = -88,
        e = -87,
        x = -85,
        Z = -67,
        z = -66,
        O = -32,
        N = -29,
        n = -28
    };

    static bool Empty(const char* string) {
        int i = 0;
        while (string[i] != '\0') {
            char c = string[i];
            if (c == '"') return false;

            if (c >= 97 && c <= 122) return false;
            if (c >= 65 && c <= 90) return false;

            // Non ascii
            if (c >= -128 && c <= -18) return false;
            i++;
        }

        return true;
    }

    static bool IsVowel(const char c) {
        // a, ą, e, ę, i, o, u, y.
        // A, Ą, E, Ę, I, O, U, Y.
        switch (c) {
            case 'A':
            case 'a':
            case 'E':
            case 'e':
            case 'I':
            case 'i':
            case 'O':
            case 'o':
            case 'Y':
            case 'y':
            case char(PolishChars::A) :
            case char(PolishChars::a) :
            case char(PolishChars::E) :
            case char(PolishChars::e) :
            case char(PolishChars::O) :
            case char(PolishChars::o) :
                return true;
            default:
                return false;
        }
    }

    static char* Clear(const char* prefix, char* src, const char* suffix = "") {
        // Length without prefix and suffix
        unsigned int len = strlen(src) - strlen(prefix) - strlen(suffix);

        // Alloc memory to new ptr (+1 for '\0')
        // #REMINDER free()
        char* value = (char*)malloc((len + 1) * sizeof(char));
        if (!value) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", (len + 1) * sizeof(char));
            return nullptr;
        }

        // Copy len elements starting from length of prefix
        char* start = (src + strlen(prefix));
        memcpy(value, start, len * sizeof(char));

        // Append \0 for proper string encoding
        value[len] = '\0';

        return value;
    }

    static void PrintCenteredText(int maxX, char* text, bool clear = false) {
        printf("%c%c", Border::Vertical, Border::Vertical);
        // Fill empty space with spaces but minus 4 chars from border
        printf("%*s%*s", (int)((maxX - 4) / 2 + strlen(text) / 2), text, (int)((maxX - 4) / 2 - strlen(text) / 2), " ");
        printf("%c%c\n", Border::Vertical, Border::Vertical);
        if (clear) free(text);
    }

    static void PrintDecoCenteredBeforeOrAfterText(int maxX, int textLength, bool isBefore = true) {
        printf("%c%c", Border::Vertical, Border::Vertical);

        // Width without border,
        int width = (maxX - 4);

        // If text is not splittable in half
        int inEqualPad = textLength % 2 == 0 ? 0 : 1;

        for (int i = 0; i < (width / 2) - ((textLength - inEqualPad) / 2) - 2; i++) {
            printf(" ");
        }

        printf("%c", isBefore ? Border::CornerTopLeft : Border::CornerBottomLeft);

        for (int i = 0; i < textLength + 2; i++) {
            printf("%c", Border::Horizontal);
        }

        printf("%c", isBefore ? Border::CornerTopRight : Border::CornerBottomRight);

        for (int i = 0; i < (width / 2) - ((textLength + inEqualPad) / 2) - 2; i++) {
            printf(" ");
        }

        printf("%c%c\n", Border::Vertical, Border::Vertical);
    }

    static void PrintDecoCenteredText(int maxX, char* text, bool clear = false) {

        PrintDecoCenteredBeforeOrAfterText(maxX, strlen(text));

        printf("%c", Border::VerticalSplitRight);
        printf("%c", Border::Crossing);

        // Width without border,
        int width = (maxX - 4);

        // If text is not splittable in half
        int inEqualPad = strlen(text) % 2 == 0 ? 0 : 1;

        // Fill empty space with horizontal border but minus 4 chars from border
        // Minus 2 - last char before text must be border vertical split left, prepended with space
        for (int i = 0; i < (width / 2) - ((strlen(text) - inEqualPad) / 2) - 2; i++) printf("%c", Border::Horizontal);
        printf("%c %s %c", Border::VerticalSplitLeft, text, Border::VerticalSplitRight); // Space after text
        for (int i = 0; i < (width / 2) - ((strlen(text) + inEqualPad) / 2) - 2; i++) printf("%c", Border::Horizontal);

        printf("%c", Border::Crossing);
        printf("%c\n", Border::VerticalSplitLeft);

        PrintDecoCenteredBeforeOrAfterText(maxX, strlen(text), false);

        if (clear) free(text);
    }

    static void PrintHorizontalLine(int maxX, int type = 0) {
        /** Type:
         *  0 - top
         *  1 - mid
         *  2 - center
         */

        if (type == 0) {
            // Top left border char
            printf("%c", Border::CornerTopLeft);
            // Top split down border
            printf("%c", Border::HorizontalSplitDown);
        }
        else if (type == 1) {
            // Bottom left border char
            printf("%c", Border::CornerBottomLeft);
            // Bottom split down border
            printf("%c", Border::HorizontalSplitUp);
        }
        else if (type == 2) {
            // Vertical border split right
            printf("%c", Border::VerticalSplitRight);
            // Crossing border
            printf("%c", Border::Crossing);
        }

        for (int x = 2; x < maxX - 2; x++) {
            // Horizontal border char
            printf("%c", Border::Horizontal);
        }

        if (type == 0) {
            // Top right border char
            printf("%c", Border::HorizontalSplitDown);
            // Top split down border
            printf("%c", Border::CornerTopRight);
        }
        else if (type == 1) {
            // Bottom left border char
            printf("%c", Border::HorizontalSplitUp);
            // Bottom split down border
            printf("%c", Border::CornerBottomRight);
        }
        else if (type == 2) {
            // Crossing border
            printf("%c", Border::Crossing);
            // Vertical border split left
            printf("%c", Border::VerticalSplitLeft);
        }

        printf("\n");
    }

    static void PrintEmptyCenter(int maxX) {
        // Vertical border char
        printf("%c%c", Border::Vertical, Border::Vertical);
        // Fill empty space with spaces but minus 4 chars from border
        printf("%*s", maxX - 4, " ");
        printf("%c%c\n", Border::Vertical, Border::Vertical);
    }

    static void PrintTableTwoRows(int maxX, char *h1, char *h2) {
        printf("%c%c", Border::Vertical, Border::Vertical);
        int len = (maxX - 2);
        int p = len / 3;
        printf("%*s", p, h1);
        printf("%*s", p, h2);
        printf("%*c%c\n", p - (len % 3) + 1, Border::Vertical, Border::Vertical);
    }

    static void Pad(unsigned targetLen, char** str) {
        int padLen = (int)targetLen - (int)strlen(*str);
        unsigned oddPad = padLen % 2 == 0 ? 0 : 1;
        padLen /= 2;

        if (padLen <= 0) return;

        unsigned newSize = (targetLen + 1) * sizeof(char);
        char* newPtr = (char*)malloc(newSize);

        strcpy(newPtr, " ");
        for (int i = 1; i < padLen; i++) {
            strcat(newPtr, " ");
        }

        strcat(newPtr, *str);

        for (int i = 0; i < padLen + oddPad; i++) {
            strcat(newPtr, " ");
        }

        free(*str);
        *str = newPtr;
    }

    static char* MakeSpinResultString(char* left, char* middle, char* right) {

        // Left and right must be same length
        unsigned targetLen = strlen(right);
        if (strlen(left) > strlen(right)) {
            targetLen = strlen(left);
            TextUtils::Pad(targetLen, &right);
        }
        else {
            TextUtils::Pad(targetLen, &left);
        }

        unsigned int added = strlen("   | >  < |   ");
        unsigned int total = added + strlen(left) + strlen(middle) + strlen(right);

        char l = (char)Special::FullArrowLeft;
        char r = (char)Special::FullArrowRight;

        char* str = (char*)malloc((total + 1) * sizeof(char));
        strcpy(str, " ");
        strcat(str, left);
        strcat(str, "  | ");
        strncat(str, &r, 1);
        strcat(str, " ");
        strcat(str, middle);
        strcat(str, " ");
        strncat(str, &l, 1);
        strcat(str, " |  ");
        strcat(str, right);
        strcat(str, " ");
        str[total] = '\0';

        free(left);
        free(middle);
        free(right);

        return str;
    }

    static char* CombineStrings(char* left, char* right, bool clearLeft = false, bool clearRight = false) {
        char* newStr = (char*)malloc((strlen(left) + strlen(right) + 1) * sizeof(char));

        strcpy(newStr, left);
        strcat(newStr, right);

        if (clearLeft) free(left);
        if (clearRight) free(right);

        return newStr;
    }

    static char ToLowerCase(char c) {
        if (c <= 'Z' && c >= 'A')
            return c - ('Z' - 'z');

        switch (c) {
            case char(PolishChars::A) :
                return char(PolishChars::a);

            case char(PolishChars::L) :
                return char(PolishChars::l);

            case char(PolishChars::X) :
                return char(PolishChars::x);

            case char(PolishChars::E) :
                return char(PolishChars::e);

            case char(PolishChars::C) :
                return char(PolishChars::c);

            case char(PolishChars::S) :
                return char(PolishChars::s);

            case char(PolishChars::Z) :
                return char(PolishChars::z);

            case char(PolishChars::O) :
                return char(PolishChars::o);

            case char(PolishChars::N) :
                return char(PolishChars::n);

            default:
                return c;
        }
    }

    static char* Cover(char* string, Vector* guessed) {

        char* newString = (char*)malloc((strlen(string) + 1) * sizeof(char));
        strcpy(newString, string);

        for (int i = 0; i < strlen(newString); i++) {
            char c = newString[i];

            bool hide = true;

            if (!TextUtils::IsValidChar(c)) {
                hide = false;
            }
            else {
                for (int j = 0; j < guessed->Length(); j++) {
                    if (TextUtils::ToLowerCase((*(char*)guessed->Get(j))) == TextUtils::ToLowerCase(c)) {
                        hide = false;
                        break;
                    }
                }
            }

            if (!hide) continue;

            newString[i] = (char)TextUtils::Special::BigDash;
        }

        return newString;
    }

    static bool IsValidChar(char c) {
        if (c >= -128 && c <= -87) return true;
        if (c >= -85 && c <= -83) return true;
        if (c >= -75 && c <= -72) return true;
        if (c >= -67 && c <= -66) return true;
        if (c >= -58 && c <= -57) return true;
        if (c >= -47 && c <= -40) return true;
        if (c >= -32 && c <= -18) return true;
        if (c >= -5 && c <= -3) return true;
        if (c >= 65 && c <= 90) return true;
        if (c >= 97 && c <= 122) return true;
        return false;
    }

    static bool CheckChoice(const char* choice) {
        switch (*choice) {
            case 'T':
            case 't':
            case 'y':
            case 'Y':
                return true;
            default:
                return false;
        }
    }

    static char* IToS(long long i) {
        char* tmp = (char*)malloc(2048 * sizeof(char));
        if (!tmp) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", (2048 * sizeof(char)));
            return nullptr;
        }

        sprintf(tmp, "%lli", i);
        char* final = (char*)malloc((strlen(tmp) + 1) * sizeof(char));

        if (!final) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", (strlen(tmp) + 1) * sizeof(char));
            return nullptr;
        }

        sprintf(final, "%lli", i);
        free(tmp);

        return final;
    }

    static bool EqualsIgnoreCase(char* first, char* second) {

        if (strlen(first) != strlen(second)) return false;

        for (int i = 0; i < strlen(first); i++) {
            if (TextUtils::ToLowerCase(first[i]) != TextUtils::ToLowerCase(second[i])) return false;
        }

        return true;
    }

    static bool AccumulateSameChars(char *string, char toAccumulate, int &count) {

        for (int i = 0; i < strlen(string); i++) {
            if (TextUtils::ToLowerCase(*(string + i)) == TextUtils::ToLowerCase(toAccumulate)) count++;
        }

        return count > 0;
    }

    static bool Contains(Vector *charsVector, char toFind) {
        for (int i = 0; i < charsVector->Length(); i++) {
            if (*((char*) charsVector->Get(i)) == toFind) return true;
        }
        return false;
    }
};

class Game {
    int screenWidth;

    struct SubItemOnWheel {
        char* name;
        int value;

        void Free() const {
            if (name) free(name);
        }

        [[nodiscard]] char* ToString() const {
            return TextUtils::IToS(value);
        }
    };

public:
    struct Phrase {
        char* ptrToCategory;
        char* value;

        void Free() {
            free(this->value);
            this->ptrToCategory = nullptr;
        }
    };

    struct ItemOnWheel {
        char type;
        short round;
        unsigned int value;
        char* name;
        Vector* items;

        void Defaults() {
            type = 'C';
            round = -1;
            value = 0;
            name = nullptr;
            items = nullptr;
        }

        void Free() const {
            if (name) free(name);
            if (!items) return;
            while (items->Length()) {
                SubItemOnWheel* it = (SubItemOnWheel*)items->Pop();
                it->Free();
                free(it);
            }
            free(items);
        };

        [[nodiscard]] SubItemOnWheel* GetSubItem(unsigned int childIndex) const {
            return (SubItemOnWheel*)this->items->Get(childIndex);
        }

        [[nodiscard]] char* ToString() const {
            char* str;
            switch (type) {
                case 'V':
                    str = (char*)malloc((strlen("NAGRODA") + 1) * sizeof(char));
                    strcpy(str, "NAGRODA");
                    return str;
                case 'B':
                    str = (char*)malloc((strlen("BANKRUT") + 1) * sizeof(char));
                    strcpy(str, "BANKRUT");
                    return str;
                default:
                case 'S':
                    str = (char*)malloc((strlen("STOP") + 1) * sizeof(char));
                    strcpy(str, "STOP");
                    return str;
                case 'C':
                    return TextUtils::IToS(value);
            }
        }
    };

    struct SpinResult {
        ItemOnWheel* left;
        ItemOnWheel* middle;
        ItemOnWheel* right;
    };
private:

    struct Player {
        char* nickname;
        unsigned wallet;
        unsigned temporaryWallet;
        Vector* valuables;

        void Free() const {
            free(nickname);
            free(valuables);
        }

        void AddValuable(SubItemOnWheel* valuable) const {
            valuables->Push(valuable);
        }

        [[nodiscard]] unsigned TotalWallet() const {
            unsigned val = 0;
            for (int v = 0; v < valuables->Length(); v++) {
                ItemOnWheel* itemOnWheel = (ItemOnWheel*)valuables->Get(v);
                val += itemOnWheel->value;
            }

            return wallet + val;
        }

        void Init() {
            valuables = (Vector*)malloc(sizeof(Vector));
            valuables->Init();
            temporaryWallet = 0;
            wallet = 0;
        }
    };

    ItemOnWheel* GetItem(unsigned int index) {
        return (ItemOnWheel*)this->wheel->Get(index);
    }

    SpinResult GetRandWheel(unsigned short currentRound) {
        SpinResult result{};

        // Get next index
        int index = rand() % this->wheel->Length();
        result.middle = this->GetItem(index);

        while (result.middle->round != -1 && result.middle->round != currentRound) {
            index = rand() % this->wheel->Length();
            result.middle = this->GetItem(index);
        }

        int indexLeft = index - 1;
        if (indexLeft == -1) indexLeft = this->wheel->Length() - 1;
        result.left = this->GetItem(indexLeft);

        while ((result.left->round != -1 && result.left->round != currentRound) && result.left == result.middle) {
            indexLeft--;
            if (indexLeft == -1) indexLeft = this->wheel->Length() - 1;
            result.left = this->GetItem(indexLeft);
        }

        int indexRight = index + 1;
        if (indexRight == this->wheel->Length()) indexRight = 0;
        result.right = this->GetItem(indexRight);

        while ((result.right->round != -1 && result.right->round != currentRound) && result.right == result.middle ) {
            indexRight++;
            if (indexRight == this->wheel->Length()) indexRight = 0;
            result.right = this->GetItem(indexRight);
        }

        return result;
    }

    static SubItemOnWheel* GetRandValuable(ItemOnWheel* itemOnWheel) {

        if (itemOnWheel->type == 'V') {
            unsigned int index = rand() % itemOnWheel->items->Length();
            return itemOnWheel->GetSubItem(index);
        }
        else {
            return nullptr;
        }
    }

    Phrase* GetRandPhrase() {
        return (Phrase*)this->phrases->Get(rand() % this->phrases->Length());
    }

    char* GetUIString(unsigned int index) {
        return (char*)this->UIStrings->Get(index);
    }

    Player* GetPlayer(unsigned index) {
        return (Player*)this->players->Get(index);
    }

    bool LoadDataFromFile(const char* file, int maxBufferPerLine = 2048) {
        FILE* data = fopen(file, "r");
        if (!data) {
            fprintf(stderr, "Can't open file: %s\n", file);
            return false;
        }

        // Max single line length
        char* buffer = (char*)malloc(maxBufferPerLine * sizeof(char));

        char section = ' ';
        unsigned int lastCategoryIndex = 0;
        unsigned int lastItemIndex = 0;
        unsigned int lastSubItemIndex = 0;
        while (fgets(buffer, maxBufferPerLine, data)) {
            // Ignore lines with # at front
            if (buffer[0] == '#') continue;

            // Replace \n with \0
            buffer[strlen(buffer) - 1] = '\0';

            // Ignore empty lines
            if (TextUtils::Empty(buffer)) continue;

            // Now is section containing phrases
            if (buffer[0] == 'p') {
                section = 'p';
                continue;
            }
            else if (buffer[0] == 'w') {
                // Now is section containing wheel
                section = 'w';
                continue;
            }
            else if (buffer[0] == 'u') {
                section = 'u';
                continue;
            }

            if (section == 'p') {
                // Categories begins with '  -' char, so this will be item
                if (buffer[4] != '-') {
                    char* category = TextUtils::Clear("  \"", buffer, "\":");
                    this->categories->Push(category);
                    lastCategoryIndex++;
                }
                else {
                    // Everything else must be phrase value
                    char* value = TextUtils::Clear("    - \"", buffer, "\"");

                    // Assign to category
                    Phrase* phrase = (Phrase*)malloc(sizeof(Phrase));
                    if (!phrase) {
                        fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", sizeof(Phrase));
                        return false;
                    }

                    // Assign last loaded category
                    phrase->ptrToCategory = (char*)this->categories->Get(lastCategoryIndex - 1);
                    phrase->value = value;

                    this->phrases->Push(phrase);
                }
            }
            else if (section == 'w') {

                if (buffer[2] == '-') {
                    // Push empty
                    ItemOnWheel* itemOnWheel = (ItemOnWheel*)malloc(sizeof(ItemOnWheel));
                    itemOnWheel->Defaults();
                    this->wheel->Push(itemOnWheel);

                    // Next item
                    lastItemIndex++;
                }

                if (buffer[4] == 'v') {

                    char* v = TextUtils::Clear("  - value: ", buffer);
                    ((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->type = 'C';
                    ((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->value = strtol(v, nullptr, 10);
                    free(v);
                }
                else if (buffer[4] == 't') {

                    ((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->type = buffer[10];
                }
                else if (buffer[4] == 'r') {

                    char* v = TextUtils::Clear("    round: ", buffer);
                    ((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->round = (short)strtol(v, nullptr, 10);
                    free(v);
                }
                else if (buffer[4] == 'i') {
                    // New sub items list
                    lastSubItemIndex = 0;

                    ((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->items = (Vector*)malloc(sizeof(Vector));
                    ((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->items->Init();
                }
                else if (buffer[6] == '-') {
                    // Next sub item
                    // Push empty
                    SubItemOnWheel* subItemOnWheel = (SubItemOnWheel*)malloc(sizeof(SubItemOnWheel));
                    ((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->items->Push(subItemOnWheel);
                    lastSubItemIndex++;
                }

                if (buffer[8] == 'n') {
                    char* v = TextUtils::Clear("      - name: \"", buffer, "\"");
                    ((SubItemOnWheel*)((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->items->Get(lastSubItemIndex - 1))->name = v;
                }
                else if (buffer[8] == 'v') {

                    char* v = TextUtils::Clear("        value: ", buffer);
                    ((SubItemOnWheel*)((ItemOnWheel*)this->wheel->Get(lastItemIndex - 1))->items->Get(lastSubItemIndex - 1))->value = strtol(v, nullptr, 10);
                    free(v);
                }

            } else if (section == 'u') {
                char* v = TextUtils::Clear("  - \"", buffer, "\"");
                this->UIStrings->Push(v);
            } else {
                fprintf(stderr, "missing section: %s\n", buffer);
            }
        }

        free(buffer);
        return true;
    }

    int round = 0;
    Vector* categories;
    Vector* phrases;
    Vector* wheel;
    Vector* UIStrings;
    Vector* players;
public:


    bool Init(int x, const char* file) {
        this->screenWidth = x;

        this->categories = (Vector*)malloc(sizeof(Vector));
        if (!this->categories) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", sizeof(Vector));
            return false;
        }
        this->categories->Init();

        this->phrases = (Vector*)malloc(sizeof(Vector));
        if (!this->phrases) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", sizeof(Vector));
            return false;
        }
        this->phrases->Init();

        this->wheel = (Vector*)malloc(sizeof(Vector));
        if (!this->wheel) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", sizeof(Vector));
            return false;
        }
        this->wheel->Init();

        this->UIStrings = (Vector*)malloc(sizeof(Vector));
        if (!this->UIStrings) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", sizeof(Vector));
            return false;
        }
        this->UIStrings->Init();

        this->players = (Vector*)malloc(sizeof(Vector));
        if (!this->players) {
            fprintf(stderr, "Not enough memory, exceed by %llu bytes total\n", sizeof(Vector));
            return false;
        }
        this->players->Init();

        if (!this->LoadDataFromFile(file)) {
            return false;
        }

        srand(time(nullptr));
        return true;
    }

    void FreeRam() {

        while (this->phrases->Length()) {
            Phrase* removed = (Phrase*)this->phrases->Pop();
            removed->Free();
            free(removed);
        }
        free(this->phrases);

        while (this->categories->Length()) {
            free(this->categories->Pop());
        }
        free(this->categories);

        while (this->wheel->Length()) {
            ItemOnWheel* removed = (ItemOnWheel*)this->wheel->Pop();
            removed->Free();
            free(removed);
        }
        free(this->wheel);

        while (this->UIStrings->Length()) {
            free(this->UIStrings->Pop());
        }
        free(this->UIStrings);

        while (this->players->Length()) {
            Player* removed = (Player*)this->players->Pop();
            removed->Free();
            free(removed);
        }
        free(this->players);
    }

    void SetupPlayers() {

        system("cls");
        TextUtils::PrintHorizontalLine(this->screenWidth);
        TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(23));
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(24));
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintHorizontalLine(this->screenWidth, 1);
        printf("%s", this->GetUIString(3));

        int pCount = 0;
        char* input = (char*)malloc(1024 * sizeof(char));
        fgets(input, 1024, stdin);
        *(input + strlen(input) - 1) = '\0';

        while ((pCount = strtol(input, nullptr, 10)) <= 0) {
            system("cls");
            TextUtils::PrintHorizontalLine(this->screenWidth);
            TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(23));
            TextUtils::PrintEmptyCenter(this->screenWidth);
            TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(24));
            TextUtils::PrintEmptyCenter(this->screenWidth);
            TextUtils::PrintHorizontalLine(this->screenWidth, 1);
            printf("%s", this->GetUIString(3));
            fgets(input, 1024, stdin);
            *(input + strlen(input) - 1) = '\0';
        }

        free(input);

        for (int i = 0; i < pCount; i++) {
            Player* p = (Player*)malloc(sizeof(Player));
            p->Init();

            char* tmp = (char*)malloc(2048 * sizeof(char));
            *tmp = '\0';

            while (strlen(tmp) < 1) {
                system("cls");
                TextUtils::PrintHorizontalLine(this->screenWidth);
                TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(23));
                TextUtils::PrintEmptyCenter(this->screenWidth);
                TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(24));
                TextUtils::PrintEmptyCenter(this->screenWidth);
                TextUtils::PrintHorizontalLine(this->screenWidth, 1);
                printf("%s(%i): ", this->GetUIString(4), i + 1);

                fgets(tmp, 2048, stdin);
                *(tmp + strlen(tmp) - 1) = '\0';
            }

            p->nickname = (char*)malloc((strlen(tmp) + 1) * sizeof(char));
            strcpy(p->nickname, tmp);
            free(tmp);

            this->players->Push(p);
        }

    }

    void ShowWheel(int playerIndex, SpinResult* result, SubItemOnWheel* item = nullptr) {

        system("cls");
        TextUtils::PrintHorizontalLine(this->screenWidth, 0);
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(0));
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(6), this->GetPlayer(playerIndex)->nickname), true);
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintDecoCenteredText(this->screenWidth, TextUtils::MakeSpinResultString(result->left->ToString(), result->middle->ToString(), result->right->ToString()), true);

        TextUtils::PrintEmptyCenter(this->screenWidth);

        if (result->middle->type == 'C') {
            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(result->middle->ToString(), this->GetUIString(9), true), true);
        }
        else if (result->middle->type == 'V') {
            TextUtils::PrintCenteredText(this->screenWidth, item->name);
            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(TextUtils::CombineStrings(this->GetUIString(10), item->ToString(), false, true), this->GetUIString(9), true), true);
        }
        else if (result->middle->type == 'B') {
            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetPlayer(playerIndex)->nickname, this->GetUIString(11)), true);
        }
        else if (result->middle->type == 'S') {
            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetPlayer(playerIndex)->nickname, this->GetUIString(12)), true);
        }

        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintHorizontalLine(this->screenWidth, 1);

        system("pause >nul");
    }

    static int CompareByWallet(const void* a, const void* b) {

        unsigned int wa = (*((Player**)a))->TotalWallet();
        unsigned int wb = (*((Player**)b))->TotalWallet();

        if (wa > wb) return -1;
        if (wa < wb) return 1;
        return 0;
    }

    static int CompareByWalletNegative(const void* a, const void* b) {
        return -1 * CompareByWallet(a, b);
    }

    void ReorderPlayersBySpin() {

        // No need to
        if (this->players->Length() == 1) {
            return;
        }

        for (int i = 0; i < this->players->Length(); i++) {
            system("cls");
            TextUtils::PrintHorizontalLine(this->screenWidth);
            TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(5));
            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(6), this->GetPlayer(i)->nickname), true);
            TextUtils::PrintEmptyCenter(this->screenWidth);
            TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(7));
            TextUtils::PrintHorizontalLine(this->screenWidth, 1);

            system("pause >nul");

            SpinResult result = GetRandWheel(1);
            if (result.middle->type == 'V') {
                SubItemOnWheel* item = GetRandValuable(result.middle);
                this->GetPlayer(i)->wallet = item->value;
                this->ShowWheel(i, &result, item);
            } else if (result.middle->type == 'C') {
                this->GetPlayer(i)->wallet = result.middle->value;
                this->ShowWheel(i, &result);
            } else {
                this->ShowWheel(i, &result);
            }
        }

        qsort(this->players->Begin(), this->players->Length(), sizeof(Player *), Game::CompareByWallet);

        // Clear money
        for (int i = 0; i < this->players->Length(); i++) {
            this->GetPlayer(i)->wallet = 0;
        }
    }

    void NextRound() {
        this->round++;

        system("cls");
        TextUtils::PrintHorizontalLine(this->screenWidth);

        Vector* guessed = (Vector*)malloc(sizeof(Vector));
        guessed->Init();

        Game::Phrase* phrase = this->GetRandPhrase();
        TextUtils::PrintDecoCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(2), phrase->ptrToCategory), true);
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, TextUtils::Cover(phrase->value, guessed), true);
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(18), TextUtils::IToS(this->round), false, true), true);
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintHorizontalLine(this->screenWidth, 1);
        system("pause >nul");

        bool end = false;
        int attempts = 0, winnerIndex = 0;
        while (!end && attempts < 10) {
            for (int i = 0; i < this->players->Length() && !end; i++) {
                Player* p = this->GetPlayer(i);

                if (this->players->Length() > 1) {
                    system("cls");
                    TextUtils::PrintHorizontalLine(this->screenWidth);
                    TextUtils::PrintEmptyCenter(this->screenWidth);
                    TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(22), p->nickname), true);
                    TextUtils::PrintEmptyCenter(this->screenWidth);
                    TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
                    TextUtils::PrintEmptyCenter(this->screenWidth);
                    TextUtils::PrintHorizontalLine(this->screenWidth, 1);
                    system("pause >nul");
                }

                bool nextPlayer = false;
                while (!nextPlayer && !end && attempts < 10) {

                    SpinResult result = GetRandWheel(this->round);
                    SubItemOnWheel* item = nullptr;
                    if (result.middle->type == 'V') {
                        item = GetRandValuable(result.middle);
                        this->ShowWheel(i, &result, item);
                    } else {
                        this->ShowWheel(i, &result);
                    }

                    if (result.middle->type == 'S' || result.middle->type == 'B') {
                        if (this->players->Length() == 1) attempts++;
                        nextPlayer = true;
                        this->GetPlayer(i)->temporaryWallet = 0;
                        continue;
                    }

                    char* userInput = (char*)malloc(2048 * sizeof(char));
                    *userInput = ' '; // Invalid
                    bool showWrongMSG = false;
                    bool showDuplicateMSG = false;

                    char* covered = TextUtils::Cover(phrase->value, guessed);

                    // This must be correct char
                    bool buy = false;
                    bool checks = false;
                    while (!checks && (TextUtils::Contains(guessed, *userInput) || !TextUtils::IsValidChar(*userInput) || (TextUtils::IsVowel(*userInput) && !buy))) {
                        system("cls");
                        TextUtils::PrintHorizontalLine(this->screenWidth);
                        TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(TextUtils::CombineStrings(this->GetUIString(18), TextUtils::IToS(this->round), false, true), TextUtils::CombineStrings(this->GetUIString(27), TextUtils::IToS(attempts + 1), false, true)), true);
                        TextUtils::PrintEmptyCenter(this->screenWidth);
                        TextUtils::PrintDecoCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(2), phrase->ptrToCategory), true);
                        TextUtils::PrintEmptyCenter(this->screenWidth);
                        TextUtils::PrintEmptyCenter(this->screenWidth);
                        TextUtils::PrintCenteredText(this->screenWidth, covered);
                        TextUtils::PrintEmptyCenter(this->screenWidth);
                        if (showDuplicateMSG || showWrongMSG) {
                            TextUtils::PrintHorizontalLine(this->screenWidth, 2);
                            if (showWrongMSG) TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(25));
                            if (showDuplicateMSG) TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(26));
                            TextUtils::PrintHorizontalLine(this->screenWidth, 2);

                            showWrongMSG = false;
                            showDuplicateMSG = false;
                        }
                        TextUtils::PrintEmptyCenter(this->screenWidth);
                        TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetPlayer(i)->nickname, this->GetUIString(14)), true);
                        TextUtils::PrintHorizontalLine(this->screenWidth, 2);

                        char* money = (char*) malloc(1);
                        strcpy(money, "\0");

                        for (int ij = 0; ij < this->players->Length(); ij++) {
                            money = TextUtils::CombineStrings(money, this->GetUIString(44), true);
                            money = TextUtils::CombineStrings(money, this->GetPlayer(ij)->nickname, true);
                            money = TextUtils::CombineStrings(money, this->GetUIString(45), true);
                            money = TextUtils::CombineStrings(money, TextUtils::IToS(this->GetPlayer(ij)->temporaryWallet), true, true);
                            money = TextUtils::CombineStrings(money, this->GetUIString(9), true);
                            money = TextUtils::CombineStrings(money, this->GetUIString(46), true);

                            if (strlen(money) + (ij * strlen(this->GetUIString(47)) + 20) >= this->screenWidth) {
                                TextUtils::PrintCenteredText(this->screenWidth, money);
                                strcpy(money, "\0");
                                continue;
                            }

                            if (ij + 1 < this->players->Length()) {
                                money = TextUtils::CombineStrings(money, this->GetUIString(47), true);
                            }
                        }

                        TextUtils::PrintCenteredText(this->screenWidth, money);
                        TextUtils::PrintHorizontalLine(this->screenWidth, 1);

                        printf("%s", this->GetUIString(15));
                        fgets(userInput, 2048, stdin);
                        *(userInput + strlen(userInput) - 1) = '\0';

                        if (strlen(userInput) > 1) {
                            checks = true;
                            continue;
                        }

                        if (TextUtils::Contains(guessed, *userInput)) {
                            showDuplicateMSG = true;
                            continue;
                        }

                        if (!TextUtils::IsVowel(*userInput)) {
                            showWrongMSG = true;
                            continue;
                        }

                        system("cls");
                        TextUtils::PrintHorizontalLine(this->screenWidth);
                        TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(28));
                        TextUtils::PrintEmptyCenter(this->screenWidth);
                        TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(userInput, this->GetUIString(16)), true);
                        TextUtils::PrintHorizontalLine(this->screenWidth, 1);
                        printf("%s", this->GetUIString(17));

                        char* choice = (char*) malloc(10 * sizeof(char));
                        fgets(choice, 10, stdin);
                        *(choice + strlen(choice) - 1) = '\0';

                        if (!TextUtils::CheckChoice(choice)) {
                            continue;
                        }

                        if (p->temporaryWallet < 200) {
                            system("cls");
                            TextUtils::PrintHorizontalLine(this->screenWidth);
                            TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(28));
                            TextUtils::PrintEmptyCenter(this->screenWidth);
                            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(TextUtils::CombineStrings(this->GetUIString(20), TextUtils::IToS(200 - p->wallet), false, true), this->GetUIString(9), true), true);
                            TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
                            TextUtils::PrintHorizontalLine(this->screenWidth, 1);
                            system("pause >nul");
                            continue;
                        }

                        p->temporaryWallet -= 200;
                        buy = true;
                    }

                    attempts++;

                    // Add single char
                    if (strlen(userInput) == 1) {
                        char *ptr = (char *) malloc(sizeof(char));
                        *ptr = *userInput; // Copy for Vector
                        guessed->Push(ptr);

                        int count = 0;
                        if (TextUtils::AccumulateSameChars(phrase->value, *ptr, count)) {
                            if (TextUtils::IsVowel(*ptr)) {
                                // Show result
                                continue;
                            } else {
                                system("cls");
                                TextUtils::PrintHorizontalLine(this->screenWidth);

                                if (result.middle->type == 'C') {
                                    unsigned int v = count * result.middle->value;
                                    p->temporaryWallet += v;
                                    TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(TextUtils::CombineStrings(this->GetUIString(29), TextUtils::IToS(v), false, true), this->GetUIString(30), true), true);
                                } else {
                                    p->AddValuable(item);
                                    TextUtils::PrintCenteredText(this->screenWidth, item->name);
                                    TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(TextUtils::CombineStrings(this->GetUIString(10), item->ToString(), false, true), this->GetUIString(31), true), true);
                                }

                                TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
                                TextUtils::PrintHorizontalLine(this->screenWidth, 1);
                                system("pause >nul");
                            }
                        } else {
                            TextUtils::PrintHorizontalLine(this->screenWidth);
                            TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(37));
                            TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
                            TextUtils::PrintHorizontalLine(this->screenWidth, 1);
                            system("pause >nul");
                            nextPlayer = true;
                        }
                    } else {
                        system("cls");
                        TextUtils::PrintHorizontalLine(this->screenWidth);
                        // Whole phrase
                        if (TextUtils::EqualsIgnoreCase(phrase->value, userInput)) {
                            TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(32));
                            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(TextUtils::CombineStrings(this->GetUIString(33), TextUtils::IToS(p->temporaryWallet), false, true), this->GetUIString(34), true), true);
                            p->wallet += p->temporaryWallet;
                            end = true;
                            winnerIndex = i;
                        } else {
                            TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(35));
                            TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(36));
                            if (this->players->Length() == 1) attempts++;
                            nextPlayer = true;
                        }
                        TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
                        TextUtils::PrintHorizontalLine(this->screenWidth, 1);
                        system("pause >nul");
                    }

                    free(covered);
                    free(userInput);
                }
            }
        }

        system("cls");
        TextUtils::PrintHorizontalLine(this->screenWidth);
        TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(50));

        if (end) {
            TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(49), this->GetPlayer(winnerIndex)->nickname));
        } else {
            TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(48));
        }

        TextUtils::PrintCenteredText(this->screenWidth, TextUtils::CombineStrings(this->GetUIString(1), phrase->value), true);

        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, this->GetUIString(8));
        TextUtils::PrintHorizontalLine(this->screenWidth, true);
        system("pause >nul");

        // Clear
        while (guessed->Length()) free(guessed->Pop());
        free(guessed);

        // Clear per round wallet
        for (int i = 0; i < this->players->Length(); i++) {
            this->GetPlayer(i)->temporaryWallet = 0;
        }
    }

    void PrintGameSummary() {
        system("cls");
        // Reorder by cash
        qsort(this->players->Begin(), this->players->Length(), sizeof(Player *), Game::CompareByWallet);

        TextUtils::PrintHorizontalLine(this->screenWidth);
        TextUtils::PrintDecoCenteredText(this->screenWidth, this->GetUIString(38));

        Player* w = this->GetPlayer(0);

        char* str = TextUtils::CombineStrings(this->GetUIString(39), w->nickname);
        str = TextUtils::CombineStrings(str, this->GetUIString(40), true);
        str = TextUtils::CombineStrings(str, TextUtils::IToS(w->TotalWallet()), true, true);
        str = TextUtils::CombineStrings(str, this->GetUIString(9), true);

        if (w->valuables->Length()) {
            str = TextUtils::CombineStrings(str, this->GetUIString(43), true);
        }

        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintCenteredText(this->screenWidth, str, true);

        for (int j = 0; j < w->valuables->Length(); j++) {
            SubItemOnWheel *sb = (SubItemOnWheel *) w->valuables->Get(j);

            char* str2 = TextUtils::CombineStrings(sb->name, this->GetUIString(10));
            str2 = TextUtils::CombineStrings(str2, TextUtils::IToS(sb->value), false, true);
            str2 = TextUtils::CombineStrings(str2, this->GetUIString(9));

            TextUtils::PrintCenteredText(this->screenWidth, str2, true);
        }

        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintTableTwoRows(this->screenWidth, this->GetUIString(41), this->GetUIString(42));

        for (int i = 0; i < this->players->Length(); i++) {
            Player* p = this->GetPlayer(i);

            char* s = TextUtils::IToS(p->TotalWallet());
            TextUtils::PrintTableTwoRows(this->screenWidth, p->nickname, s);
            free(s);
        }
        TextUtils::PrintEmptyCenter(this->screenWidth);
        TextUtils::PrintHorizontalLine(this->screenWidth, 1);
    }

    void ReorderPlayersByMinimalCash() {
        qsort(this->players->Begin(), this->players->Length(), sizeof(Player *), Game::CompareByWalletNegative);
    }
};

int main(int argc, char** argv) {
    Game game{};
    if (!game.Init(120, "data.yml")) {
        return 1;
    }

    game.SetupPlayers();
    game.ReorderPlayersBySpin();

    for (int round = 0; round < 3; round++) {
        game.NextRound();
        game.ReorderPlayersByMinimalCash();
    }

    game.PrintGameSummary();
    game.FreeRam();
    system("pause >nul");
}
