
## Example DB Setup

```sql
CREATE VIEW `GithubProjectsFilteres` AS SELECT * FROM `GithubProjectUnfiltered` WHERE `GITHUB_LICENSE` IN ('apache-2.0', 'bsd-2-clause', 'bsd-3-clause', 'cc0-1.0', 'mit', 'unlicense', 'zlib');
```
