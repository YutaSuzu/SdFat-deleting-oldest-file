# SdFat-deleting-oldest-file
---
Based on following site.
https://forum.arduino.cc/index.php?topic=530065.0

I changed Opennext line. "O_WRITE" does not work.  
Change to "O_RDONLY"  
But remove() does not work with "O_RDONLY", So reopen by char with getName.
