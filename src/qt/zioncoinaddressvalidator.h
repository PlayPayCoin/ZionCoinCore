// Copyright (c) 2011-2014 The Zioncoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef Zioncoin_QT_ZioncoinADDRESSVALIDATOR_H
#define Zioncoin_QT_ZioncoinADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ZioncoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ZioncoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Zioncoin address widget validator, checks for a valid Zioncoin address.
 */
class ZioncoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ZioncoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // Zioncoin_QT_ZioncoinADDRESSVALIDATOR_H
