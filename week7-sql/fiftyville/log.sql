-- Keep a log of any SQL queries you execute as you solve the mystery.

-- This seems like a fun problem lets go
-- I know that the event took place on July 28 2024 on Humphrey Street

-- I want to know how the first table looks like so
SELECT * FROM crime_scene_reports

-- Lets look at the day of the theft
SELECT * FROM crime_scene_reports WHERE year = 2024 AND month = 7 AND day = 28;
-- I can see that case with id 295 is Theft of the CS50 duck, interviews mention bakery, time is 10:15am

-- Next table is interviews so lets see if there is something about the duck
SELECT * FROM interviews WHERE transcript LIKE "%duck%"
-- No luck

-- Lets see atm transactions on that day
SELECT * FROM atm_transactions WHERE year = 2024 AND month = 7 AND day = 28;
-- Ok, there are lots of records, I will try to filter location
SELECT * FROM atm_transactions WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location LIKE "%Humphrey%";
-- Even after looking at the Humphrey location there are still lots of transactions

-- I know that bakery is important so I will look at security logs from that day
SELECT * FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28;
-- Lots of data, I need to get more details I think that i might have missed something in the interviews


SELECT transcript FROM interviews WHERE year = 2024 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";
-- Now this query is much better, I have some important info now

-- Ten minutes of the theft thief got into a car in bakery parking lot
-- Earlier that morning thief was withdrawing money on Legget Street
-- Thief was talking with someone on the phone after leaving the bakery,
-- they were planning to take the earliest flight next day out of Fiftyvile.
-- Thief asked to pruchase the flight ticket

-- First parking lot
SELECT * FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute >=15 AND minute <= 25;
-- To much cars but one of them is linked to theft
SELECT id FROM people
WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute >=15 AND minute <= 25);
-- I got list of persons from parking lot

-- Money withdraw
SELECT account_number FROM atm_transactions WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location LIKE "%Legget%";
-- There is no hour of transaction in this table but I know that one of these account numbers owners is the thief
SELECT person_id FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location LIKE "%Legget%");
-- I got list of persons that withdrew money that day on Legget

-- Thief was on the phone after leaving bakery
SELECT caller FROM phone_calls WHERE year = 2024 AND month = 7 AND day = 28;
-- lots of calls
SELECT id FROM people
WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2024 AND month = 7 AND day = 28);

-- Now i will intersect all of the previous person ids to find the thief
SELECT id FROM people
WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25)
INTERSECT
SELECT person_id FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location LIKE "%Legget%")
INTERSECT
SELECT id FROM people
WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2024 AND month = 7 AND day = 28);
-- Unfortunately I got two ids 514354,686048 Diana and Bruce

-- Lets check their passport ids
SELECT id, name, passport_number FROM people
WHERE id IN (514354, 686048);
-- passport numbers are 3592750733, 5773159633
SELECT * FROM flights
WHERE id IN (SELECT flight_id FROM passengers WHERE passport_number IN (3592750733, 5773159633))
AND year = 2024 AND month = 7 AND day = 28;
-- They both took flight on the next day but Diana flew out later than Bruce and I know that they wanted to take the earliest flight
-- Bruce has to be the thief, flight destination is LaGuardia Airport in New York

-- I know that Bruce called his partner
SELECT receiver FROM phone_calls
WHERE caller = (SELECT phone_number FROM people WHERE id = 686048)
AND year = 2024 AND month = 7 AND day = 28;
-- He called 4 people that day
SELECT * FROM people
WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE caller = (SELECT phone_number FROM people WHERE id = 686048) AND year = 2024 AND month = 7 AND day = 28);
-- I got passport numbers of three people and one is suspicious because passport number is NULL, guy is called Robin
SELECT * FROM atm_transactions
WHERE account_number IN (SELECT account_number FROM bank_accounts WHERE person_id = 864400)
AND year = 2024 AND month = 7 AND day = 29;
-- Robin spent 90$ on the day of the flight so I assume that Robin is Bruce's partner

