import detect_english
from vigenere_cipher import convert

def main(ciphertext):
    msg_and_key = dict_atack(ciphertext)
    if msg_and_key != None:
        return msg_and_key[0], msg_and_key[1]
    else:
        return None

def dict_atack(ciphertext):
    with open('dictionary.txt') as f:
        words = f.readlines()

    for word in words:
        decrypted_message = convert('d', ciphertext, word.strip(), False)
        if detect_english.is_english(decrypted_message, word_percentage=40):
            print('possible encryption break')
            print(f'key {word}: {decrypted_message[:100]}')
            print(f'Possible break with key {word}. Press D to finish, or anything else to continue: ')
            response = input('>')

            if response.upper().startswith('D'):
                return decrypted_message, word.strip()
    # if the atack is unsuccessful return an empty list
    return []
