import vigenere_cipher
import frequency_analysis
import itertools
import detect_english
import dictionary_atack_vcipher

LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
MAX_KEY_LENGHT = 16
NUM_OF_FREQUENT_LETTERS = 4

def main():
    msg = input('Enter your ciphertext for decryption here: ')

    *msg_and_key, = hack_vcipher(msg)
    if msg_and_key != []:
        print(f'successfully hacked the cipher with the key - \'{msg_and_key[1]}\'\n')
        print(f'decrypted message - {msg_and_key[0]}\n')
    else:
        print('failed to hack decryption')

def get_repeated_sequence_spacings(message):
    ''' goes trough the message finding all repeating 3 to 5-letter repeated sequences.
    returns a dictionary with the keys being the sequence and the values the list of spacings between them.'''
    # clean the message from all non-letter characters
    message = ''.join(ch for ch in message.upper() if ch in LETTERS)

    sequenceSpacings = {}
    for sequenceLen in range(3, 6):
        for start in range(len(message) - sequenceLen): # start index of every sequence. 0 - len(message) - 3, 4 or 5
            sequence = message[start:start + sequenceLen]
            for index in range(start + sequenceLen, len(message) - sequenceLen): # index is the start index of the sequence. Iterate trough all the elements after this index + the lenght of the sequence until the end and look for matches
                if message[index:index+sequenceLen] == sequence:
                    if sequence not in sequenceSpacings: # if a match is found and the sequence is not in the sequenceSpacings dictionary yet, assign a list to the sequence key
                        sequenceSpacings[sequence] = []
                    sequenceSpacings[sequence].append(index - start) # and then add the difference of the starting index of the sequence and the starting index of the matched sequence - the spacing between the two sequences

    return sequenceSpacings

def reduce_factors(spacing):
    ''' reduce the factors of a spacing to a range up to MAX_KEY_LENGHT '''
    if spacing < 2:
        return []
    factors = []
    for n in range(2, MAX_KEY_LENGHT+1):
        if spacing % n == 0:
            factors.append(n)

    return factors

def get_factors_count(seq_factors):
    ''' return a list of tuples ot two items, the first one being the factor
        and the second one being it's count, all the first items are essentially the likely key lenghts '''
    factors_count = {}
    for sequence in seq_factors:
        factors = seq_factors[sequence]
        for factor in factors:
            if factor not in factors_count:
                factors_count[factor] = 0
            factors_count[factor] += 1

    factors_count_list = []
    for factor in factors_count:
        if factor <= MAX_KEY_LENGHT:
            factors_count_list.append( (factor, factors_count[factor]) )

    factors_count_list.sort(key=lambda x: x[1], reverse=True)

    return factors_count_list

def find_probable_key_lenghts(message):
    ''' message undergoes kasiski analysis
        we find all repeated sequences of 3-5 letters long and the spacings between every occurence
        then we get all the factors of all the spacings and get their count (if the factor is between 1-MAX_KEY_LENGHT)
        and then we return the list of most probable key lenghts '''
    repeated_seq_spacings = get_repeated_sequence_spacings(message)

    seq_factors = {}
    for sequence in repeated_seq_spacings:
        seq_factors[sequence] = []
        for spacing in repeated_seq_spacings[sequence]:
            seq_factors[sequence].extend(reduce_factors(spacing))

    factors_count = get_factors_count(seq_factors)
    probable_key_lenghts = [pair[0] for pair in factors_count]

    return probable_key_lenghts

def get_nth_letters(message, start, key_lenght):
    ''' return a sequence of every nth letter in the message.
        get_nth_letters('ABCABCABC', 0, 3) -> AAA '''
    letters = []
    for index in range(start, len(message), key_lenght):
        letters.append(message[index])

    return ''.join(letters)

def try_with_key(message, probable_key):
    ''' try to decrypt the ciphertext with a probable_key lenght.
        first we split the ciphertext into nth letters sequences based on the probable_key
        then we get the 4 highest match scores on every letter sequence with the alphabet letters A-Z
        second we use itertools.product(range(NUM_OF_FREQUENT_LETTERS), repeat=probable_key) to get all the indexes in lexicographical order -> (0, 0, 0), (0, 0, 0) ... (3, 3, 3) for probable_key = 3, (0, 0, 0, 0, 0, 0) ... (3, 3, 3, 3, 3, 3) for probable_key = 6
        then we create a possible key from the highest match score keys of all nth letter sequences with (index is 0-2) allFreqScores[index][indexes[index]][1] -> allFreqScores[0][(0, 0, 0)[0][0]], [1](0, 0, 0)[1][0] ... [0](3, 3, 3)[0][0], [1](3, 3, 3)[1], [2](3, 3, 3)[2][2]
        and if the decrypted message with the possible key is close to english ask the user if the text is encrypted. If it is return the text if not continue with the next key
        '''
    messageUp = ''.join(lett for lett in message.upper() if lett in LETTERS)
    ''' allFreqScores is a list with probable_key amount of lists that have every letter in the alphabet paired with it's frequency match score on the nth letters of the ciphertext
        we do this so we can find out the NUM_OF_FREQUENT_LETTERS highest frequency scores on each of the probable_key nth letter sequences with a key from A-Z'''
    allFreqScores = []
    for n in range(probable_key):
        letter_sequence = get_nth_letters(messageUp, n, probable_key)

        freq_scores = []
        for key in LETTERS:
            decrypted_message = vigenere_cipher.convert('d', letter_sequence, key, False)
            key_frequency = (key, frequency_analysis.englishFreqMatchScore(decrypted_message))
            freq_scores.append(key_frequency)
        freq_scores.sort(key=lambda x: x[1], reverse=True)

        allFreqScores.append(freq_scores[:NUM_OF_FREQUENT_LETTERS])

    for indexes in itertools.product(range(NUM_OF_FREQUENT_LETTERS), repeat=probable_key):
        possible_key = ''
        for index in range(probable_key):
            possible_key += allFreqScores[index][indexes[index]][0]
        print(f'attempt to break with key {possible_key}')

        possible_decryption = vigenere_cipher.convert('d', message, possible_key.upper(), randomized=False)
        if detect_english.is_english(possible_decryption):
            print(f'Possible break with key {possible_key}. Press D to finish, or anything else to continue: ')
            print(possible_decryption[:200])

            cont = input('>').lower()
            if cont.startswith('d'):
                return possible_decryption, possible_key

    return []

def hack_vcipher(message):
    ''' try to break the vigenere cipher with all most probable key lenghts from the kasisky analysis
        if they fail try to brute force trough every single key lenght up to MAX_KEY_LENGHT '''
    probable_key_lengths = find_probable_key_lenghts(message)

    keys = ', '.join(str(key_lenght) for key_lenght in probable_key_lengths)
    print(f'based on kasiski analysis the most likely key lenghts are {keys}\n')

    # attempt with kasiski's analysis keys
    for probable_key in probable_key_lengths:
        print(f'attempt to hack the ciphertext with key lenght {probable_key} - possible combinations {NUM_OF_FREQUENT_LETTERS ** probable_key}\n')

        *msg_and_key, = try_with_key(message, probable_key)
        if msg_and_key != []:
            return msg_and_key[0], msg_and_key[1]

    print()
    print('kasiski analysis method failed to decrypt the message\n')
    print('attempting to brute force trough the key lenghts\n')
    # brute force attempt
    for key in range(2, MAX_KEY_LENGHT):
        print(f'attempt to  brute force the ciphertext with key lenght {key} - possible combinations {NUM_OF_FREQUENT_LETTERS ** key}')

        *msg_and_key, = try_with_key(message, key)
        if msg_and_key != []:
            return msg_and_key[0], msg_and_key[1]

    return []

if __name__ == '__main__':
    main()
