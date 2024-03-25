ΕΡΓΑΣΙΑ 4

ΚΩΣΤΑΝΤΙΝΟΣ ΜΥΡΤΟΛΛΑΡΙ (sdi2000148@di.uoa.gr)
AΜ : 1115202000148

ΜΙΧΑΗΛ ΧΡΗΣΤΟΣ ΝΑΒΑΡΟ ΑΜΑΡΓΙΑΝΟΣ (sdi2000151@di.uoa.gr)
ΑΜ : 1115202000151 


Η εργασία υλοποιεί όλες τις βασικές λειτουργίες που υποδεικνύονται στην εκφώνηση, χωρίς την υλοποίηση των bonus λειτουργιών.

Tο πρόγραμμα αποτελείται απο πέντε πηγαία αρχεία και τέσσερα αρχεία επικεφαλίδας.

Τo αρχείο modes.c περιέχει συναρτήσεις που επιτελούν τις βασικές εντολές της ipl (πχ. read , write , if/else , while ....). To αρχείο lists.c , από την άλλη ,
περιέχει συναρτήσεις πάνω στην διαχείρηση λιστών και το αρχείο check.c περιέχει δύο συναρτήσεις πάνω στον έλεγχο συμβολοσειρών. Τέλος, το words.c περιέχει μια 
συνάρτηση που είναι υπεύθυνη για την διάσπαση κάθε γραμμής στα ουσιώδη της στοιχεία (parsing).

Για την φύλαξη και προσπέλαση των μεταβλητών της ipl γίνεται η χρήση μιας απλά συνδεδεμένης λίστας. Κάθε κόμβος της λίστας περιλαμβάνει 
ένα πίνακα χαρακτήρων για την φύλαξη του ονόματος της μεταβλητής και μια ακέραια μεταβλητή για την φύλαξη της τιμής της μεταβλητής. H λίστα 
ονομάζεται memory_list. Επιπλέον, για την υλοποίηση της επιλογής "-v" στην κλήση του διερμηνέα γίνεται χρήση μιας λίστας με όνομα line_store_list. 
Τέλος, κάθε κόμβος της λίστας αποθηκεύει σε έναν πίνακα χαρακτήρων μια γραμμή του αρχείου που περιέχει ipl κώδικα και σε μια ακέραια 
μεταβλητή τον αριθμό που αντιστοιχεί στην γραμμή. 

Στην διαχείρηση εμφολευμένων εντολών (while , if) χρησιμοποιείται μια αναδρομική συνάρτηση με όνομα rec_main, που βρίσκεται στο modes.c και μεταβλητές τύπου
fpos_t, για την μετάβαση απο την μια γραμμή του ipl κώδικα στην άλλη (πχ. μετά τις εντολές μιας while)

Το πρόγραμμα μπορεί να μεταγλωτιστεί ως εξής : gcc -o ipli main.c modes.c lists.c check.c words.c

Όλα τα τμήματα τις εργασίας υλοποιήθηκαν απο κοινού μέσω τηλεδιάσκεψης.

* Επιπλέον, έχει υλοποιηθεί η πρώτη εργασία σε ipl. Στο διάστημα [100 , 100.000] απαιτούνται 2 λεπτά για την εκτέλεση.

