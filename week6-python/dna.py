import csv
import sys
from pprint import pprint


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("error")

    # TODO: Read database file into a variable
    STRs = {}
    rows = []
    with open(sys.argv[1]) as databaseFile:
        reader = csv.DictReader(databaseFile)
        for str in reader.fieldnames[1:]:
            STRs[str] = None
        for row in reader:
            rows.append(row)


    #pprint(STRs)
    
    # TODO: Read DNA sequence file into a variable
    sequence = None
    with open(sys.argv[2]) as sequenceFile:
        sequence = sequenceFile.read() 

    #print(sequence)


    # TODO: Find longest match of each STR in DNA sequence
    for str in STRs.keys():
        STRs[str] = longest_match(sequence, str)

    #pprint(STRs)
    #pprint(rows)

    # TODO: Check database for matching profiles
    for row in rows:
        ok = 0
        for str in STRs.keys():
            if int(row[str]) == STRs[str]:
                ok +=1
        if(ok == len(STRs)):
            print(row["name"]) 
            return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
