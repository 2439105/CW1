/*2471065c
*sp Exercise 1a
*“This is my own work as defined in the Academic Ethics agreement I have signed.”
*/


#include <stdio.h>
//#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "date.h"

struct date
{
    int day_;
};

static int is_leap_year(short year)
{
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

static short max_day_of_month_year(short year, short month)
{
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        if (is_leap_year(year))
            return 29;
        else
            return 28;
    default:
        return 0;
    }
}


/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date* date_create(char* datestr)
{
    if (!datestr)
        return NULL;

    char* p = datestr;
    if (p == NULL) {
        fprintf(stderr, "0 - Illegal input line: %s", datestr);
        return NULL;
    }

    int day = 0;
    for (int i = 0;i < 2;++i)
    {
        if (*p < '0' || *p > '9' || *p == '\0')
        {
            fprintf(stderr, "1 - Illegal input line: %s", datestr);
            return NULL;
        }
        day = day * 10 + (int)(*p - '0');
        ++p;
    }
    if (day >= 32 || day <= 0)
    {
        fprintf(stderr, "2 - Illegal input line: %s", datestr);
        return NULL;
    }

    if (*p != '/')
    {
        fprintf(stderr, "3 - Illegal input line: %s", datestr);
        return NULL;
    }
    ++p;

    short month = 0;
    for (int i = 0;i < 2;++i)
    {
        if (*p < '0' || *p > '9' || *p == '\0')
        {
            fprintf(stderr, "4 - Illegal input line: %s", datestr);
            return NULL;
        }
        month = month * 10 + (int)(*p - '0');
        ++p;
    }
    if (month >= 13 || month <= 0)
    {
        fprintf(stderr, "5 - Illegal input line: %s month:%d", datestr, month);
        return NULL;
    }

    if (*p != '/')
    {
        fprintf(stderr, "6 - Illegal input line: %s", datestr);
        return NULL;
    }
    ++p;

    short year = 0;
    for (int i = 0;i < 4;++i)
    {
        if (*p < '0' || *p > '9' || *p == '\0')
        {
            fprintf(stderr, "7 - Illegal input line: %s", datestr);
            return NULL;
        }
        year = year * 10 + (int)(*p - '0');
        ++p;
    }
    if (*p != '\0')
    {
        fprintf(stderr, "8 - Illegal input line: %s", datestr);
        return NULL;
    }

    short day_limit = max_day_of_month_year(year, month);
    if (day > day_limit)
    {
        fprintf(stderr, "9 - Illegal input line: %s", datestr);
        return NULL;
    }

	Date * date_st = (Date *)malloc(sizeof(Date));
	if (!date_st)
	{
        fprintf(stderr, "10 - Illegal input line: %s", datestr);
		return NULL;
	}

    day = 10000 * year + 100 * month + day;
    date_st->day_ = day;

	return date_st;
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date* date_duplicate(Date* d)
{
    if (!d)
    {
        fprintf(stderr, "1 - Illegal date");
        return NULL;
    }

    Date* date_st = (Date*)malloc(sizeof(Date));
    if (!date_st)
    {
        fprintf(stderr, "1 - malloc error");
        return NULL;
    }

    date_st->day_ = d->day_;

	return date_st;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date* date1, Date* date2)
{
    if (date1->day_ > date2->day_)
        return 1;
    else if (date1->day_ == date2->day_)
        return 0;
    return -1;
}

/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date* d)
{
    if (d)
    {
        free(d);
        d = NULL;
    }
}
