#failed

KEYSIZE = 128
BLOCKSIZE = 128

Nk = 4 # Number of 32-bit words in the key
Nr = 10 # Number of rounds
Nb = 4 # Number of columns in the state
#want 10 round keys 


class matrix:
    def __init__(self, rows, cols):
        self.rows = rows
        self.cols = cols
        self.data = [[0 for _ in range(cols)] for _ in range(rows)]

    def __getitem__(self, idx):
        return self.data[idx]

    def __setitem__(self, idx, value):
        self.data[idx] = value

    def __str__(self):
        return '\n'.join([' '.join(map(str, row)) for row in self.data])
    
def keySchedule(key: int) -> list[bytes]:
    #key is 128 bit
    listKeys = []
    K = key.to_bytes(16) # Convert the key to bytes K is a list of bytes
    for i in range(0, 16, 4):
        listKeys.append(K[i:i+4])   
     

def RotWord(word: bytes) -> bytes: #max of 4 bytes
    return word[1:] + word[:1]