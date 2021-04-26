from string import ascii_uppercase as LETTERS
from random import randint
import argparse

def convert(operation, string, key, randomized):
    ''' function to convert a string into cipher text or cipher text into a string.
        Operation decision is based on the operation(e=encryption, d=decryption) '''

    # create a key | if there is no key given use LEMON
    if key is None:
        key = 'LEMON'
    key = key.upper()

    # create a message list which will store the value of the ciphered text in encryption and plain text in decryption
    message = []
    # iterate trough all the characters in the given string, and create a key index variable to keep track of which letter to use as a key
    keyIdx = 0
    for ch in string:
        # num variable will be the number corresponding to the letter used in enc/decr, set it's initial value to the index of the ch in the alphabet
        num = LETTERS.find(ch.upper())
        if num != -1: # if it is equal to -1 then it is not found in the letters, so it must be punctuation. Just append it to the list in that case
            # add the current key's index in the LETTERS variable to the num variable if encrypting, elif decryptiong subtract it
            if operation == 'e':
                num += LETTERS.find(key[keyIdx])
            elif operation == 'd':
                num -= LETTERS.find(key[keyIdx])
            # this is a wrapper so the num won't be any bigger than the len(letters), if the num is 27, then 27 % 26 = 1
            num %= len(LETTERS)

            # if the randomized flag is set, get a random int(1, 2) and make the case upper if it's 1 else lower | if the flag is not set keep the case of the original message
            if randomized is False:
                if ch.isupper():
                    ltr = LETTERS[num].upper()
                elif ch.islower():
                    ltr = LETTERS[num].lower()
            else:
                case = randint(1, 2)
                if case == 1:
                    ltr = LETTERS[num].upper()
                else:
                    ltr = LETTERS[num].lower()
            # increment the keyidx (go to the next letter of the key), and again % is just a wrapper, so the keyIdx never exceeds the lenght of the key
            keyIdx += 1
            keyIdx %= len(key)

            message.append(ltr)
        else:
            if randomized is False:
                message.append(ch)

    # return variable is just a string with all the letters of the message list
    return ''.join(ch for ch in message)

if __name__ == '__main__':
    # create parser object and add the positional and optional arguments
    parser = argparse.ArgumentParser(prog='vigenere cipher', description='encryption/decryption of a message')

    parser.add_argument('operation', help='operation to be performed', choices=['e', 'd'])
    parser.add_argument('msg', metavar='message', help='message to be encrypted/decrypted')
    parser.add_argument('key', help='key to be used in encryption/decryption')
    parser.add_argument('-randomize', help='script will return randomized casing and no punctuation', action='store_true')

    # take the arguments passed in
    args = parser.parse_args()

    # call the convert function with the message, operation and key and print the output to the screen
    result = convert(args.operation, args.msg,  args.key, args.randomize)
    print(result)