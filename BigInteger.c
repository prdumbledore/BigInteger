#include <stdio.h>
#include "stdlib.h"
#include "BigInteger.h"

// Экземпляры функций
BigInteger digitsToBint(Digits*, Sign);
BigInteger doDivision(BigInteger, BigInteger, bool);
Sign divMultSign(Sign, Sign);
MultiplicationTable getTable(BigInteger);
char* intToString(int);

// Копирование числа
BigInteger bintCpy(BigInteger source) {
    return (BigInteger) { digitsCpy(source.digits), source.sign};
}

// Представление листа с цифрами в виде числа
BigInteger digitsToBint(Digits *digits, Sign sign) {
    BigInteger bigint;
    bigint.digits = digits;
    bigint.sign = sign;
    return bigint;
}

// Представление строки в виде числа
BigInteger stringToBint(char *string) {
    BigInteger bigint;
    if (string[0] == '0' || (string[0] == '-' && string[1] == '0')) {
        bigint.sign = positive;
        bigint.digits = stringToDigits("0");
    }
    else if (string[0] == '-') {
        bigint.sign = negative;
        bigint.digits = stringToDigits(string + 1);
    }
    else {
        bigint.sign = positive;
        bigint.digits = stringToDigits(string);
    }
    return bigint;
}

// Представления числа в виде строки
char* bintToString(BigInteger biginteger) {
    if (isEmpty(*(biginteger.digits))) {
        char *arr = malloc(2);
        arr[0] = '0';
        arr[1] = 0;
        return arr;
    }
    else if (biginteger.sign == positive)
        return digitsToString(reverseDigits(biginteger.digits), 0);
    else
        return digitsToString(reverseDigits(biginteger.digits), '-');
}

// Операция сложения
BigInteger bintAdd(BigInteger a, BigInteger b) {
    Digits *result = createDigits(0), *shorter, *longer;
    Sign s;
    int min, max, i, carry = 0;
    if (!a.digits || !b.digits)
        return stringToBint("0");
    if (a.sign == positive && b.sign == positive)
        s = positive;
    else if (b.sign == negative && a.sign == negative)
        s = negative;
    else if (a.sign == negative) {
        a.sign = positive;
        return bintSub(b, a);
    }
    else {
        b.sign = positive;
        return bintSub(a, b);
    }

    if (a.digits->count == b.digits ->count) {
        min = (int) a.digits->count;
        max = min;
        shorter = a.digits;
        longer = b.digits;
    }
    else {
        shorter = getShorter(a.digits, b.digits, &min);
        longer = getLonger(a.digits, b.digits, &max);
    }
    for (i = 0; i < max; i++) {
        int shorter_data = 0, longer_data = (int) getByIndex(longer, i)->data, new_data;
        if (i < min)
            shorter_data = (int) getByIndex((Digits * )shorter, i)->data;
        new_data = shorter_data + longer_data + carry;
        carry = 0;
        while (new_data > 9)
        {
            new_data -= 10;
            carry++;
        }
        pushBack(result, (char)new_data);
    }
    if (carry > 0)
        pushBack(result, (char)carry);
    return digitsToBint(result, s);
}

// Операция вычитания
BigInteger bintSub(BigInteger a, BigInteger b) {
    int i=0;
    BigInteger result = { createDigits(0), 0};
    if (!a.digits || !b.digits)
        return stringToBint("0");
    if (a.sign != b.sign) {
        b.sign = a.sign ? negative : positive;
        return bintAdd(a,b);
    }
    if (bintEqual(a, b))
        return stringToBint("0");

    if (bintGreaterThan((BigInteger) { a.digits,  positive}, (BigInteger) { b.digits, positive})) {
        bool carry = false;
        Node *head1 = a.digits->HEAD, *head2 = b.digits->HEAD, *curr;
        Digits *reversed;
        while (head1) {
            int tmp;
            tmp = head2 ? head1->data - head2->data : head1->data;
            if (carry) {
                tmp--;
                carry = false;
            }
            if (tmp < 0) {
                carry = true;
                tmp += 10;
            }
            pushBack(result.digits, (char) tmp);
            head1 = head1->next;
            if (head2)
                head2 = head2->next;
            i++;
        }
        for (; i<a.digits->count; i++) {
            pushBack(result.digits, head1->data);
            head1 = head1->next;
        }
        reversed = reverseDigits(result.digits);
        curr = reversed->HEAD;
        while (curr && curr->data == 0) {
            curr = curr->next;
            popFront(reversed);
        }
        result.digits = reverseDigits(result.digits);
        result.sign = a.sign;
    }
    else {
        result = bintSub(b, a);
        result.sign = !a.sign;
    }
    return result;
}

// Операция умножения
BigInteger bintMul(BigInteger a, BigInteger b) {
    if (!a.digits || !b.digits)
        return stringToBint("0");
    if (isZero(a) || isZero(b))
        return stringToBint("0");
    Sign s = divMultSign(a.sign, b.sign);
    MultiplicationTable  t = getTable(a);
    BigInteger result = { createDigits(0),  s};
    int i;

    for (i = 0; i < b.digits->count; i++) {
        BigInteger value = bintCpy(t.values[(unsigned) getByIndex(b.digits, i)->data]);
        int j = i;
        value.sign = s;
        while (j > 0) {
            pushFront(value.digits, 0);
            j--;
        }
        BigInteger freeable = result;
        result = bintAdd(result, value);
        freeBint(freeable);
        freeBint(value);
    }
    for (int j = 0; j < 10; j++)
        freeBint(t.values[j]);
    return result;
}

// Операция целочисленного деления
BigInteger bintDiv(BigInteger a, BigInteger b) {
    if (!a.digits || !b.digits)
        return stringToBint("0");
    if (isZero(b))
        return intToBint(0);
    if (bintGreaterThan((BigInteger) { b.digits,  positive}, (BigInteger) { a.digits,  positive}))
        return intToBint(0);
    return doDivision(a, b, 1);
}

// Операция вычисления остатка
BigInteger bintMod(BigInteger a, BigInteger b) {
    if (!a.digits || !b.digits)
        return stringToBint("0");
    if (isZero(b))
        return intToBint(0);
    return doDivision(a, b, 0);
}

/**
 * Оптимизированное деление:
 * Умножаем B на 10, пока оно не станет меньше A,
 * затем делаем вычитания и после дополнительных вычислений получаем окончательный ответ
 */
BigInteger doDivision(BigInteger a, BigInteger b, bool div) {
    Sign bs = b.sign;
    int i;
    unsigned difference=1;
    BigInteger tmp_a = {digitsCpy(a.digits), positive}, sub_final, ten_laipsnis, ten,
            freeable, result, tmp2, j, vienas = intToBint(1);
    if (isZero(a))
        return intToBint(0);
    tmp_a.sign = positive;
    b.sign = positive;
    ten_laipsnis = intToBint(1);

    if (a.digits->count > b.digits->count)
        difference = a.digits->count - b.digits->count;
    ten = intToBint(10);
    // мы нашли самый большой индекс 10, такой что 10^index*b < a
    for (i=0; i<difference-1; i++) {
        freeable = ten_laipsnis;
        ten_laipsnis = bintMul(ten_laipsnis, ten);
        freeBint(freeable);
    }

    tmp2 = bintMul(ten_laipsnis, b);
    freeable = tmp2;
    tmp2 = bintMul(tmp2, ten);
    freeBint(freeable);
    if (bintLesserThan(tmp2, a)) {
        freeable = ten_laipsnis;
        ten_laipsnis = bintMul(ten_laipsnis, ten);
        freeBint(freeable);
    }
    freeBint(tmp2);
    sub_final = bintMul(ten_laipsnis, b);
    j = intToBint(0);
    // делаем вычитания с sub_final
    while (bintGreaterThan(tmp_a, sub_final) || bintEqual(tmp_a, sub_final)) {
        freeable = tmp_a;
        tmp_a = bintSub(tmp_a, sub_final);
        freeBint(freeable);
        freeable = j;
        j = bintAdd(j, vienas);
        freeBint(freeable);
    }
    // умножаем временный ответ div на 10 ^ index и добавляем остаток, который делится на b
    result = bintMul(j, ten_laipsnis);
    freeable = result;
    BigInteger freeable_2 = bintDiv(tmp_a, b);
    result = bintAdd(freeable_2, result);
    if (div) {
        freeBint(tmp_a);
        result.sign = a.sign == bs || isZero(j) ? positive : negative;
        freeBint(freeable_2);
        freeBint(freeable);
        freeBint(ten);
        freeBint(vienas);
        freeBint(j);
        freeBint(sub_final);
        freeBint(ten_laipsnis);
        return result;
    }
    else {
        BigInteger freeable2 = bintDiv(tmp_a, b), freeable3 = bintMul(b, freeable2);
        freeable = tmp_a;
        tmp_a = bintSub(tmp_a, freeable3);
        freeBint(freeable);
        freeBint(freeable2);
        freeBint(freeable3);
        if ( (a.sign != bs && bs == positive && !isZero(tmp_a)) || (a.sign == bs && bs == negative))
            tmp_a.sign = negative;
        freeBint(result);
        freeBint(freeable_2);
        freeBint(ten);
        freeBint(vienas);
        freeBint(j);
        freeBint(sub_final);
        freeBint(ten_laipsnis);
        return tmp_a;
    }
}

// Проверка на ноль
bool isZero(BigInteger val) {
    Node *head;
    if (!val.digits || isEmpty(*(val.digits)))
        return false;
    head = val.digits->HEAD;
    while (head)
    {
        if (head->data != 0)
            return 0;
        head = head->next;
    }
    return 1;
}

// Определение знака при умножении или делении
Sign divMultSign(Sign a, Sign b) {
    return a == b ? positive : negative;
}

// Получение умножающей таблицы
MultiplicationTable getTable(BigInteger val) {
    int i, j;
    MultiplicationTable table;
    table.values[0] = digitsToBint(createDigits(val.digits->count), val.sign);
    table.values[1] = bintCpy(val);
    for (i = 2; i < 10; i++)
    {
        table.values[i] = bintCpy(val);
        for (j = 0; j < i - 1; j++)
        {
            BigInteger freeable = table.values[i];
            table.values[i] = bintAdd(table.values[i], val);
            freeBint(freeable);
        }
    }
    return table;
}

// Проверка на равенство двух чисел
bool bintEqual(BigInteger a, BigInteger b) {
    Node *head1, *head2;
    if (!a.digits || !b.digits)
        return false;
    if (a.digits->count == 0 || b.digits->count == 0) {
        checkErrors(DIGITS_EMPTY_EXCEPTION);
        return false;
    }
    if (a.digits->count != b.digits->count)
        return false;
    head1=a.digits->HEAD, head2=b.digits->HEAD;
    while (head1 && head2) {
        if (head1->data != head2->data)
            return false;
        head1 = head1->next;
        head2 = head2->next;
    }
    return true;
}

// Проверка a>b
bool bintGreaterThan(BigInteger a, BigInteger b) {
    Node *head1=NULL, *head2=NULL;
    bool bresult = false;
    if (!a.digits || !b.digits || isEmpty(*(a.digits)) || isEmpty(*(b.digits)))
        return false;
    if (a.digits->count == 0 || b.digits->count == 0) {
        checkErrors(DIGITS_EMPTY_EXCEPTION);
        return false;
    }
    if (a.digits->count < b.digits->count)
        return false;
    else if (a.digits->count > b.digits->count)
        return true;
    a.digits = reverseDigits(a.digits);
    b.digits = reverseDigits(b.digits);
    head1 = a.digits->HEAD;
    head2 = b.digits->HEAD;
    while (head1 && head2) {
        if ( (head1->data > head2->data && a.sign == positive) ||
             (head1->data < head2->data && b.sign == negative) ) {
            bresult = true;
            break;
        }
        else if (head1->data != head2->data)
            break;
        head1 = head1->next;
        head2 = head2->next;
    }
    a.digits = reverseDigits(a.digits);
    b.digits = reverseDigits(b.digits);
    return bresult;
}

// Проверка a<b
bool bintLesserThan(BigInteger a, BigInteger b) {
    if (!a.digits || !b.digits || isEmpty(*(a.digits)) || isEmpty(*(b.digits)))
        return false;
    return !(bintGreaterThan(a,b) || bintEqual(a, b));
}

// int в string
char* intToString(int a) {
    int tmp = a, digits=0, i, iki;
    bool negative = a < 0;
    char *arr;
    if (a == 0) {
        arr = calloc(2, 1);
        arr[0] = '0';
        return arr;
    }
    while (tmp) {
        tmp /= 10;
        digits++;
    }
    tmp = a;
    arr = calloc(digits+1+negative, 1);
    i = digits-1, iki = 0;
    if (negative) {
        i++;
        iki++;
        arr[0] = '-';
    }
    for (; i>=iki; i--) {
        char final = (char) ((char) tmp % 10);
        if (negative)
            final *= -1;
        arr[i] = (char) ((char) final + 0x30);
        tmp /= 10;
    }
    return arr;
}

// освобождение памяти от числа
void freeBint(BigInteger a) {
    freeDigits(a.digits);
}

// int в BigInteger
BigInteger intToBint(int a) {
    char *tmp = intToString(abs(a));
    BigInteger result = {stringToDigits(tmp), a < 0 ? negative : positive};
    free(tmp);
    return result;
}

// печать числа на одной строке
void printBint(BigInteger a) {
    char *tmp = bintToString(a);
    printf("%s", tmp);
    free(tmp);
}

// печать числа с переходои на новую строку
void printlnBint(BigInteger a) {
    printBint(a);
    printf("\n");
}

