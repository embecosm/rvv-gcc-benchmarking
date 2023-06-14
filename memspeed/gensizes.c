// Generate a sequence of sizes for use when evaluating memory actions

// This file is part the RVV memcpy/memset evaluation

// Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// SPDX-License-Identifier: GPL-3.0-or-later

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/// Generate a vector of prime numbers.
///
/// \note Allocates a vector, which must be freed externally when finished
///       with.
///
/// \param[in] max_prime  The largest prime sought (need not be prime).
/// \return A vector of prime numbers up to \p max_prime;
static size_t *
genprimes (size_t max_prime)
{
  int *all_nums = (int *) malloc ((max_prime + 1) * sizeof (int));

  for (size_t i = 0; i <= max_prime; i++)
    all_nums[i] = TRUE;

  // Sieve of Erastothenes
  size_t max_search = (size_t) sqrt ((double) (max_prime - 1)) + 1;
  for (size_t i = 2; i <= max_search; i++)
    if (all_nums[i] == TRUE)
      for (size_t j = 2 * i; j <= max_prime; j += i)
	all_nums [j] = FALSE;

  // How many primes did we find?
  size_t num_primes = 0;
  for (size_t i = 2; i <= max_prime; i++)
      if (all_nums[i] == TRUE)
	num_primes++;

  // Create result vector
  size_t *res = (size_t *) malloc ((num_primes + 1) * sizeof (size_t));
  size_t next_prime = 0;
  for (size_t i = 2; i <= max_prime; i++)
      if (all_nums[i] == TRUE)
	{
	  res[next_prime] = i;
	  next_prime++;
	}
  res[next_prime] = 0;		// End marker

  free (all_nums);
  return res;
}

/// Compare two size_t values
///
/// @param[in] first  Pointer to first value to compare
/// @param[in] second  Pointer to second value to compare
/// \return -1 if \p first is less than \p second, +1 if \p first is greater
///         than \p second and 0 if \p first and \p second are equal.
static int
cmpsize_t(const void *first, const void *second)
{
  size_t *a = (size_t *) first;
  size_t *b = (size_t *) second;
  return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/// Generate a sorted sequence of memory sizes.
///
/// These are generated as powers of primes up to a limit
///
/// argv[1] is the maximum prime to use
/// argv[2] is the maximum size to use
///
/// \param[in] argc  Number of arguments
/// \param[in] argv  The arguments
/// \return The return code of the program
int
main (int argc, char *argv[])
{
  if (argc != 3)
    {
      fprintf (stderr, "Usage %s <maxprime> <maxsize>\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  // We silently ignore failures
  size_t max_prime = strtoull (argv[1], NULL, 0);
  size_t max_size = strtoull (argv[2], NULL, 0);

  if (max_prime < 2)
    {
      fprintf (stderr, "ERROR: maximum prime is %lu: must be at least 2\n",
	       max_prime);
      exit (EXIT_FAILURE);
    }

  size_t *primes = genprimes (max_prime);

  // how many sizes do we have?
  size_t len = 1;		// We've need a slot for 1

  // Count the powers of primes
  for (size_t j = 0; primes [j] != 0; j++)
    for (size_t i = primes[j]; i <= max_size; i *= primes[j])
	len++;

  // Now allocate a vector of suitable size and fill it
  size_t *sizes = (size_t *) malloc (len * sizeof (size_t));
  len = 0;
  sizes[len] = 1;
  len++;

  // Add in powers of primes
  for (size_t j = 0; primes [j] != 0; j++)
    for (size_t i = primes[j]; i <= max_size; i *= primes[j])
      {
	sizes[len] = i;
	len++;
      }

  qsort (sizes, len, sizeof (sizes[0]), &cmpsize_t);

  for (size_t i = 0; i < (len - 1); i++)
    printf ("%lu ", sizes[i]);

  printf ("%lu\n", sizes[len - 1]);

  free (sizes);
  free (primes);
}

// Local Variables:
// mode: C
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
