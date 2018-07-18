#!/usr/bin/env python3

import sqlite3

query = """
SELECT CLASSIFICATION, COUNT(*)
    FROM `GithubProjectUnfiltered`
    WHERE STATUS == "IMPLEMENTED"
    GROUP BY CLASSIFICATION
    ORDER BY COUNT(*) DESC
"""

if __name__ == "__main__":
    conn = sqlite3.connect('database.db')

    c = conn.cursor()
    c.execute(query)

    # output table in latex format
    print(r"\begin{tabular}{ l c }")
    print(r"  type & count \\")
    print(r"  \hline")
    for line in c.fetchall():
        print(r"  {type} & {count} \\".format(type=line[0], count=line[1]))
    print(r"\end{tabular}")
