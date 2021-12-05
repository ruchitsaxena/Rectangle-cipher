#include <bits/stdc++.h>
using namespace std;

typedef unsigned short bit16;

// sbox
const int sbox[] = {0x6, 0x5, 0xc, 0xa, 0x1, 0xe, 0x7, 0x9, 0xb, 0x0, 0x3, 0xd, 0x8, 0xf, 0x4, 0x2};
// round constant
const int rcon[] = {0x01, 0x02, 0x04, 0x09, 0x12, 0x05, 0x0B, 0x16, 0x0C, 0x19, 0x13, 0x07, 0x0F, 0x1F, 0x1E, 0x1C, 0x18, 0x11, 0x03, 0x06, 0x0D, 0x1B, 0x17, 0x0E, 0x1D};

// For printing each row of 16 bits
void printbit16(bit16 n)
{
    int i;
    for(i=15;i>=0;i--)
        cout << (((1 << i) & n) ? 1 : 0) << " ";
    cout << '\n';
}

// For printing complete state
void printstate(vector<bit16> st)
{
    int i;
    for(bit16 n : st)
    {
        for(i=15;i>=0;i--)
        cout << (((1 << i) & n) ? 1 : 0) << " ";
        cout << '\n';
    }
}


// Key Updation
void keyupdate(vector<bit16> &key,int round)
{
    int i,j;
    // sub column of key
    for(i=0;i<4;i++)
    {
        int newval = sbox[ (((key[0] >> i) & 1) | ((key[1] >> i) & 1) << 1 | ((key[2] >> i) & 1) << 2 | ((key[3] >> i) & 1) << 3) ];
        for(j=0;j<4;j++)
            key[j] = (key[j] & ~(1 << i)) | ((newval >> j) & 1) << i;
    }

    //Fiestal Transformation
    bit16 tmpRow0 = key[0];
    key[0] = ((key[0] >> 8) | (key[0] << 8)) ^ key[1];
    key[1] = key[2];
    key[2] = key[3];
    key[3] = ((key[3] >> 4) | (key[3] << 12)) ^ key[4];
    key[4] = tmpRow0;

    // add 5-bit round constant
    key[0] = key[0] ^ rcon[round];
}

// Adding round key
void addroundkey(vector<bit16> &state , vector<bit16> &key)
{
    int i;
    for(i=0;i<4;i++)
    state[i] ^= key[i];
}


// Sub column of state
void subcolumn(vector<bit16> &state)
{
    vector<bit16> t(12 , 0) , b(4,0);

    t[1] = ~state[1];
    t[2] = state[0] & t[1];
    t[3] = state[2] ^ state[3];
    b[0] = t[2] ^ t[3];

    t[5] = state[3] | t[1];
    t[6] = state[0] ^ t[5];
    b[1] = state[2] ^ t[6];
    t[8] = state[1] ^ state[2];

    t[9] = t[3] & t[6];
    b[3] = t[8] ^ t[9];
    t[11] = b[0] | t[8];
    b[2] = t[6] ^ t[11];

    // cout << "Sub-column :\n";
    for(int i = 0; i < 4; i++)
    {
        // cout << hex << b[i] << "\n";
        state[i] = b[i];
    }
}

// Doing Shift rows
void shiftrows(vector<bit16> &state)
{
    state[1] = (state[1] << 1) | (state[1] >> 15);
    state[2] = (state[2] << 12) | (state[2] >> 4);
    state[3] = (state[3] << 13) | (state[3] >> 3);
}

// Complete encrypt function
void encrypt(vector<bit16> &state ,vector<bit16> &key)
{
    int i;
    
    for(int j=0;j<25;j++)
    {
        addroundkey(state , key);
        subcolumn(state);
        shiftrows(state);
        keyupdate(key , j);
        cout << "-----------------------------------------\n\n";
        cout << "Round : " << j << "\n";
        cout << "\nState : \n";
        printstate(state);
        cout << "\nKey : \n";
        printstate(key);
        cout << "\n-----------------------------------------\n";
        // If want to print in hex

        /* cout << "-----------------------------------------\n\n";
        cout << "state : " << "\n";
        for(i=0;i<4;i++)
        cout << hex << state[i] << "\n";
        cout << "\n";
        cout << "Key : " << "\n";
        for(i=0;i<5;i++)
        cout << hex << key[i] << "\n";

        cout << "\n";
        cout << "-----------------------------------------\n\n";*/
    }

    addroundkey(state , key);
    // cout << "\nFinal State : " << "\n";
    // printstate(state);
}

void io()
{
    cout << "Enter message in binary : \n";
    vector<bit16> state(4 , 0) , key(5 , 0);
    int i;
    // Inputting Message
    for(i=0;i<4;i++)
    {
        bit16 n;
        for(int j=0;j<16;j++)
        {
            int x;
            cin >> x;
            if(x == 1)
            n = n & 1;
            
            n <<= 1;
        }
        state[i] = n;
    }

    cout << "Enter the key in binary : \n";
    for(i=0;i<5;i++)
    {
        bit16 n;
        for(int j=0;j<16;j++)
        {
            int x;
            cin >> x;
            if(x == 1)
            n = n & 1;
            
            n <<= 1;
        }
        state[i] = n;
    }

    encrypt(state , key);
    cout << "\n";
    cout << "\nFinal State :\n";
    printstate(state);
    cout << "\n";

}

string convbit16tochars(bit16 n)
{
    string r;
    
    r = char((n & (0xff << 8)) >> 8);
    
    r += char(n & (0xff << 0));
    return r;
}



string encryptstr(string s)
{
    string res;
    vector<bit16> state(4 , 0) , key(5 , 0);
    int i, n = s.length();
    for(i=0;i<n;i+=4)
    {
        // cout << i << " : \n";
        state[i%4] = i < n ? (bit16)s[i] : 0;
        state[(i+1)%4] = i + 1 < n ? (bit16)s[i+1] : 0;
        state[(i+2)%4] = i + 2 < n ? (bit16)s[i+2] : 0;
        state[(i+3)%4] = i + 3 < n ? (bit16)s[i+3] : 0;
        // printstate(state);
        // cout << "\n";
        // encrypt(state , key);
        // printstate(state);
        // cout << "\n";
        // cout << (char)state[i%4] << (char)state[(i+1)%4] << (char)state[(i+2)%4] << (char)state[(i+3)%4];
        res += convbit16tochars(state[i%4]);
        res += convbit16tochars(state[(i+1)%4]);
        res += convbit16tochars(state[(i+2)%4]);
        res += convbit16tochars(state[(i+3)%4]);
        // cout << "\n";
    }
    return res;
}



int main()
{
    // Taking input and printing output
    io();
    
    return 0;
}
