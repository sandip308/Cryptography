#include <bits/stdc++.h>
#define ul unsigned int
#define uc unsigned char
using namespace std;

//key expansion
// S-Box
uc SBOX[256] =
    {
        // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// RCON TABLE   1      2     3    4     5     6     7     8     9     10
uc RCON[11] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// start AES operations
void Add_Round_Key(uc *state, uc *ex_key)
{
    for (int i = 0; i < 16; i++)
    {
        state[i] ^= ex_key[i];
    }
}
void SUBBYTE(uc *state)
{
    for (int i = 0; i < 16; i++)
    {
        state[i] = SBOX[state[i]];
    }
}
void SHIFT_ROW(uc *state)
{
    uc temp[16] = {0};
    int fst_shift = 0, snd_shift = 1;
    int trd_shift = 2, frth_shift = 3;
    // 1st column 0 shift
    for (int i = 0; i < 4; i++)
    {
        temp[fst_shift] = state[fst_shift % 16];
        fst_shift += 4;
    }
    //2nd column 1 shift
    for (int i = 0; i < 4; i++)
    {
        temp[snd_shift] = state[(snd_shift + 4) % 16];
        snd_shift += 4;
    }
    //3rd column 2 shift
    for (int i = 0; i < 4; i++)
    {
        temp[trd_shift] = state[(trd_shift + 8) % 16];
        trd_shift += 4;
    }
    //4rth column 3 shift
    for (int i = 0; i < 4; i++)
    {
        temp[frth_shift] = state[(frth_shift + 12) % 16];
        frth_shift += 4;
    }
    //Update state matrix
    for (int i = 0; i < 16; i++)
    {
        state[i] = temp[i];
    }
}
void mixing_column(uc *temp)
{
    uc a[4], b[4];
    uc c, d;
    for (c = 0; c < 4; c++)
    {
        a[c] = temp[c];
        d = (uc)((signed char)temp[c] >> 7); //If high bit of temp[c] is set
        b[c] = temp[c] << 1;                 //remove high bit xor with 0x1B
        b[c] ^= 0x1B & d;
    }
    temp[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; //2*a0 + 3*a1 + a2 + a3
    temp[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
    temp[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    temp[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
}
void MIX_COLUMN(uc *state)
{
    uc temp[4];
    int k = 0, p = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp[j] = state[k];
            k++;
        }
        mixing_column(temp);
        for (int j = 0; j < 4; j++)
        {
            state[p] = temp[j];
            p++;
        }
    }
}
void Encryption(uc *msg, uc *ex_key, uc *encrypt_msg)
{
    uc state[16];
    for (int i = 0; i < 16; i++)
    {
        state[i] = msg[i];
    }
    Add_Round_Key(state, ex_key); // initial Round
    //Go to main Round
    for (int i = 0; i < 9; i++)
    {
        SUBBYTE(state);
        SHIFT_ROW(state);
        MIX_COLUMN(state);
        Add_Round_Key(state, ex_key + (16 * (i + 1)));
    }
    // //Go to final round
    SUBBYTE(state);
    SHIFT_ROW(state);
    Add_Round_Key(state, ex_key + 160);
    // //copy to encrypted msg array
    for (int i = 0; i < 16; i++)
    {
        encrypt_msg[i] = state[i];
    }
}

//For key generation
void Rot_Word(uc *rot)
{
    uc p = rot[0];
    rot[0] = rot[1];
    rot[1] = rot[2];
    rot[2] = rot[3];
    rot[3] = p;
}
void Sub_byte_and_rcon(uc *sub, int count)
{
    for (int i = 0; i < 4; i++)
    {
        sub[i] = SBOX[sub[i]];
    }
    sub[0] ^= RCON[count];
}

void Expand_keys(uc key_required[16], uc Another_keys[176])
{
    for (int i = 0; i < 16; i++)
    {
        Another_keys[i] = key_required[i];
    }
    int rcon = 0;
    int first_round = 16;
    uc temp[4];
    while (first_round < 176)
    {
        // Last 4 byte of key copied to temp array
        temp[0] = Another_keys[first_round - 4 + 0];
        temp[1] = Another_keys[first_round - 4 + 1];
        temp[2] = Another_keys[first_round - 4 + 2];
        temp[3] = Another_keys[first_round - 4 + 3];

        if (first_round % 16 == 0) //for 11 times do the following process (under if block)
        {
            Rot_Word(temp);
            Sub_byte_and_rcon(temp, rcon); //Do the subbyte and rcon
            rcon++;
        }
        Another_keys[first_round + 0] = (Another_keys[first_round - 16] ^ temp[0]);
        Another_keys[first_round + 1] = (Another_keys[first_round + 1 - 16] ^ temp[1]);
        Another_keys[first_round + 2] = (Another_keys[first_round + 2 - 16] ^ temp[2]);
        Another_keys[first_round + 3] = (Another_keys[first_round + 3 - 16] ^ temp[3]);
        first_round += 4;
    }
}

int main()
{
    ofstream fout;
    string s1, s2;
    ostringstream ss;
    cout << "Enter the 16 bit key: ";
    getline(cin, s1);
    if (s1.length() == 16)
    {
        fout.open("keystore");
        for (int i = 0; i < s1.length(); i++)
        {
            ss << hex << int(s1[i]);
            s2 = ss.str();
        }
        string s4;
        for (int i = 0; i < s2.length() - 1; i++)
        {
            if (i % 2 == 0)
            {

                fout << s2[i] << s2[i + 1];
                if (i != 31)
                {
                    fout << " ";
                }
            }
            else
            {
                continue;
            }
        }
        fout.close();
    }
    else
    {
        cout << "Please enter 16 bit key...and Try again...";
        exit(0);
    }

    ifstream key_file;
    string key;
    key_file.open("keystore", ios::in | ios::binary);
    if (key_file.is_open())
    {
        getline(key_file, key);
        key_file.close();
    }
    else
    {
        cout << "Unable to open file... Try again..";
        exit(0);
    }
    istringstream get_hex(key);
    uc key_required[16];
    ul p;
    int i = 0;
    while (get_hex >> hex >> p)
    {
        key_required[i] = p;
        i++;
    }
    uc Anotherkeys[176];
    Expand_keys(key_required, Anotherkeys); //function which generate 11 round key

    char msg[17];
    cout << "Please enter the 16 bit message to encrypt: ";
    cin.getline(msg, sizeof(msg));
    int len = strlen(msg);
    uc pad_msg[16];
    if (len == 16)
    {
        for (int i = 0; i < 16; i++)
        {
            pad_msg[i] = msg[i];
        }
        uc enc_msg[16];
        Encryption(pad_msg, Anotherkeys, enc_msg);

        // //print encrypted message
        cout << "Encrypted message is: ";
        for (int i = 0; i < 16; i++)
        {
            cout << enc_msg[i] << " ";
        }
        ofstream aes;
        aes.open("encrypt");
        for (int i = 0; i < 16; i++)
        {
            aes << enc_msg[i] << " ";
        }
        aes.close();
        // cout << endl
        //      << "Encrypted message in hex: ";
        // for (int i = 0; i < 16; i++)
        // {
        //     cout << setfill('0') << setw(2) << hex << (int)enc_msg[i] << uppercase;
        //     cout << " ";
        // }
    }
    else
    {
        cout << "Please Enter 16 bit message and try again.....";
        exit(0);
    }
}