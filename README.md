# Cipher-File

`cipher-file` is a command-line tool implemented in C that uses the PRESENT block cipher algorithm to encrypt and decrypt files. It is designed to work in a Linux environment.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Disclaimer](#disclaimer)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

## Overview

The `cipher-file` program implements the PRESENT block cipher algorithm, which is a lightweight symmetric encryption algorithm known for its simplicity and efficiency. It provides encryption and decryption functionalities for files using a 80-bit key.

The program consists of two header files, `present.h` and `helpers.h`, and their corresponding implementation files, `present.c` and `helpers.c`. The `present.h` header file contains the necessary definitions and function prototypes for the PRESENT block cipher algorithm, while the `helpers.h` header file provides helper functions for file handling and user interaction.

## Features

- Encryption and decryption of files using the PRESENT block cipher algorithm
- File existence and extension validation
- User prompts for confirmation before file modification or deletion
- test.c program for debugging purposes. 

#### Test Usage

Usage: `<plaintext/ciphertext>` `<key>` `<E/D>`

```shell
gcc -o test present.c test.c
./test 0000000000000000 00000000000000000000 E
./test 4584227B38C17955 00000000000000000000 D
```
For this implementation the print_state function for writing the plaintext or ciphertext to the terminal is reversed. For instance for the test vectors at the top of the table below it will be shown as '4584227B38C17955'.

| Plaintext         | Key                    | Ciphertext         |
|------------------|------------------------|--------------------|
| 00000000 00000000 | 00000000 00000000 0000 | 5579C138 7B228445 |
| 00000000 00000000 | FFFFFFFF FFFFFFFF FFFF | E72C46C0 F5945049 |
| FFFFFFFF FFFFFFFF | 00000000 00000000 0000 | A112FFC7 2F68417B |
| FFFFFFFF FFFFFFFF | FFFFFFFF FFFFFFFF FFFF | 3333DCD3 213210D2 |


## Installation

To compile the `cipher-file` program, follow these steps:

1. Clone the repository:

   ```shell
   git clone https://github.com/aogulcemal/cipher-file.git
   ```

2. Navigate to the project directory:

   ```shell
   cd cipher-file
   ```

3. Compile the program using GCC:

   ```shell
   gcc -o cipher-file present.c helpers.c main.c
   ```

4. The `cipher-file` binary executable will be generated in the current directory.

## Usage

To encrypt a file using `cipher-file`, use the following command. If key is not provided program will generate random 80-bit key using `/dev/urandom`:

```shell
./cipher-file <filename> <key>(optional)
```

To decrypt a file:

```shell
./cipher-file <filename> <key>
```

Replace `<filename>` with the name of the file you want to decrypt, and `<key>` with the 80-bit encryption key.

## Disclaimer

This program should not be used with files that you do not want to lose, as it has not been thoroughly tested. Additionally, if you choose to use this program with a random key, it is essential to store that key in a secure location. Failure to follow these precautions may result in irreversible data loss.

## Examples

Encrypting a file:

```shell
./cipher-file test.jpg 0123456789ABCDEF0123
or
./cipher-file test.jpg
```

Decrypting a file:

```shell
./cipher-file test.enc 0123456789ABCDEF0123
```

## Contributing

Contributions to `cipher-file` are welcome! If you encounter any issues or have suggestions for improvements, please open an issue or submit a pull request on the GitHub repository: [https://github.com/aogulcemal/cipher-file](https://github.com/aogulcemal/cipher-file)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
