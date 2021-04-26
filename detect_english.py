UPPERLETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
LETTERS_AND_SPACE = UPPERLETTERS + UPPERLETTERS.lower() + ' \t\n'

def load_dictionary():
    with open('dictionary.txt') as dict_f:
        # englishWords = {}
        # for word in dict_f.readlines():
        #     word = word.strip()
        #     englishWords[word] = None
        english_words = dict.fromkeys(dict_f.read().split(), None) # shorthand for the above 4 lines :D

    return english_words

ENGLISH_WORDS = load_dictionary()

def get_english_count(message):
    ''' count how many words in the message are english
        return value is a number from 0.0 to 1.0 based on how many
        words are english from the message.
        5 english words in a text with 15 total words -> 5 / 15 = 0.33, 5 english, 10 total -> 5 / 10 = 0.5... '''
    message = message.upper()
    message = remove_non_letters(message)
    possible_words = message.split()

    if possible_words == []:
        return 0.0 # No words at all, so return 0.0.

    matches = 0
    for word in possible_words:
        if word in ENGLISH_WORDS:
            matches += 1
    return float(matches) / len(possible_words)

def remove_non_letters(message):
    return ''.join(symbol for symbol in message if symbol in LETTERS_AND_SPACE)

def is_english(message, word_percentage=20, letter_percentage=85):
    ''' check if the message contains at least 20% english words and
        at least 85% of the message must be letters or spaces to return True, else function returns False '''
    words_match = get_english_count(message) * 100 >= word_percentage

    num_letters = len(remove_non_letters(message))
    msg_letter_percentage = float(num_letters) / len(message) * 100

    letters_match = msg_letter_percentage >= letter_percentage

    return words_match and letters_match