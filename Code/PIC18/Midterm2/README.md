# Midterm
[Midterm 2 Video Link](https://youtu.be/mDQPU2lCSec)
## Notes
- This lab in the begin was a bit of a pain, the main reasoning has to do 
with the LVP mux being shared with the last two data pins for the display. 
- Some people have had success with PORTB, others (like me) have not. I am 
not entirely sure, but my rising suspicion is pointing towards something 
with the configuration bits.
- With that being said, the only change note I will add is that I used 
PORTC instead of PORTB for the display data ports. Functionally, 
everything else is sound.
- One more note, I ended up changing the size of the created char string 
for the print_int and print_long functions.

Thanks for your help professor, your advice helped me debug to the success 
point.
