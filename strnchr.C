// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/RCS/strnchr.C,v 1.1 2000/09/15 10:12:56 mhoffman Exp $

int strnchr (const char *s1, const char c)
{ int cnt=0;

  while (*s1) {
    if (*s1==c) {
      cnt++;
    }
    s1++;
  }
  return cnt;
}
