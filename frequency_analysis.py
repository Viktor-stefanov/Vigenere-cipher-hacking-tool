# ETAOIN represents arrangement of the alphabet from the most frequent letter to the least frequent one
ETAOIN = 'ETAOINSHRDLCUMWFGYPBVKJXQZ'
LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def get_letter_count(message):
    ''' returns a dictionary with each letter as the key and an integer representing it's occurences in the message
    (or the letter's count in the message) '''
    letter_count = dict.fromkeys(LETTERS, 0)

    for letter in message.upper():
        if letter in LETTERS:
            letter_count[letter] += 1

    return letter_count

def get_letter_frequency_order(message):
    # get a dictionary of the letter count in the message (check description of getLetterCount function)
    letter_count = get_letter_count(message)

    # create a dictionary with the reverse point of letterCount, make keys the count and values the letter(s) counter(that) many times in the message
    freqOfLetters = {}
    for letter in LETTERS:
        if letter_count[letter] not in freqOfLetters: # if the letter frequency  of the A-Z letter in letterCount is not a key in freqOfLetters
            freqOfLetters[letter_count[letter]] = [letter] # assign as a key to freqOfLetters the value of letterCount, and as a value of freqOfLetters a list with the letter(A-Z)
        else: # else if the letter frequency in letterCount[letter] is a key in freqOfLetters
            freqOfLetters[letter_count[letter]].append(letter) # add the current letter(A-Z) the list (that is a count of how many letters represent a certain count) created in the if block above

    # if there is more than 1 letter corresponding to a frequency, sort it in the reverse ETAOIN order, that is so we don't add a point to the matchScore by accident and make the score as low as possible, so no random high scores mess up the analysis. For instance if the order of the letters is already in ETAOIN we may add a point or two the overall score, but if it is in reverse order we do not add a random point to the overall score.
    for freq in freqOfLetters:
        freqOfLetters[freq].sort(key=ETAOIN.find, reverse=True) # key = int that we get from ETAOIN.find = AKA if the leter is E=we get 0, so it will be the first element AKA in ETAOIN order, then we reverse the result and get reversed ETAOIN order
        freqOfLetters[freq] = ''.join(freqOfLetters[freq]) # convert the result to a string

    # convert the dictionary to a list, so we can sort it by the frequency keys
    freqPairs = list(freqOfLetters.items())
    freqPairs.sort(reverse=True) # from biggest frequency to smallest

    # create a list to store all the letters by most frequent to least frequent in our message
    freqOrder = []
    for freqPair in freqPairs:
        freqOrder.append(freqPair[1])

    # finally return a sorted sequence of most frequent to least-frequent letter in the message
    return ''.join(freqOrder)

def englishFreqMatchScore(message):
    ''' match score is calculated based on how many of the 6 most frequent and 6 least frequent
        letters from the message match those of the ETAOIN order(that is english most frequent - least frequent order).
        intuitively the highest score is 12 (6 most frequent in msg == 6 most frequent in ETAIN and 6 least f in msg == 6 least f in ETAOIN)'''
    letter_frequency_order = get_letter_frequency_order(message)

    match_score = 0
    # Find how many matches of the six most frequent letters in both letter frequency orders there are:
    for common_letter in letter_frequency_order[:6]:
        if common_letter in ETAOIN[0:6]:
            match_score += 1
    # Find how many matches of the six least frequent letters in both letter frequency orders there are:
    for uncommon_letter in letter_frequency_order[-6:]:
        if uncommon_letter in ETAOIN[-6:]:
            match_score += 1

    # one liner for the above 7 lines of code, just because
    # match_score = sum(1 for common_letter in letter_frequency_order[:6] if common_letter in ETAOIN[0:6]) + sum(1 for uncommon_letter in letter_frequency_order[-6:] if uncommon_letter in ETAOIN[-6:])

    return match_score