#include <stdio.h>
#include <stdlib.h>
#include "BigInteger.h"

int main(void) {
    // Проверка mod
        BigInteger a = stringToBint("-1920141206309129219096030192491204920630102316363533"),
            b = stringToBint("12301290296039019501290312903291052910549210421");
        BigInteger c = bintMod(a, b);

        printf("\nTest mod: \n\n");
        printBint(a);
        printf(" %% ");
        printBint(b);
        printf(" =  \n");
        printlnBint(c);

        freeBint(a);
        freeBint(b);
        freeBint(c);
        // Проверка div
        a = stringToBint("19200000000000000"),
                b = stringToBint("11");
        c = bintDiv(a, b);

        printf("\nTest div: \n\n");
        printBint(a);
        printf(" / ");
        printBint(b);
        printf(" = ");
        printlnBint(c);

        freeBint(a);
        freeBint(b);
        freeBint(c);

        // Проверка add
        a = stringToBint("85949274340731304"),
                b = stringToBint("289562351485564517110334077575763123");
        c = bintAdd(a, b);

        printf("\nTest add: \n\n");
        printBint(a);
        printf(" + ");
        printBint(b);
        printf(" = ");
        printlnBint(c);

        freeBint(a);
        freeBint(b);
        freeBint(c);

        // Проверка sub
        a = stringToBint("85949274340731304"),
                b = stringToBint("289562351485564517110334077575763123");
        c = bintSub(a, b);

        printf("\nTest sub: \n\n");
        printBint(a);
        printf(" - ");
        printBint(b);
        printf(" = ");
        printlnBint(c);

        freeBint(a);
        freeBint(b);
        freeBint(c);

        // Проверка mul
        a = stringToBint("85949274340731304"),
                b = stringToBint("289562351485564517110334077575763123");
        c = bintMul(a, b);

        printf("\nTest sub: \n\n");
        printBint(a);
        printf(" * ");
        printBint(b);
        printf(" = ");
        printlnBint(c);

        freeBint(a);
        freeBint(b);
        freeBint(c);
        return 0;
    }
}