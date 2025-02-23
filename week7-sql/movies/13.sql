SELECT people.name FROM people
JOIN stars ON people.id = stars.person_id
WHERE stars.movie_id IN (
  SELECT stars.movie_id FROM stars
  JOIN people ON people.id = stars.person_id
  WHERE people.name = "Kevin Bacon" AND people.birth = 1958
)
AND (people.name != "Kevin Bacon" OR people.birth != 1958);
