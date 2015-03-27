#!/usr/bin/perl
#
# pwgen 1.4
#
#   Usage: pwgen [length] [specials]
#
#          length - an optional argument indicating the length of the password
#          specials - use '1' to force use of special characters
#
# This will generate random passwords of the specified or default length.
# Requires the Perl package Math::Random::Secure to produce
# cyptographically secure passwords.
#
# Copyright (C) 2013 - Paul E. Jones <paulej@packetizer.com>
# Permission to use, copy, modify and distribute this software is granted.
#

use strict;
use Math::Random::Secure qw(irand);

# Define the default password length
$main::default_password_length = 16;

#
# GeneratePassword
#
#     Description
#         This routine will generate a password and return it as a string.
#         By default, it will not utilize special characters like "~" in
#         passwords, but if the second argument is a 1, it will.  Note that
#         use of special characters provides only minimum additional strenth,
#         yet they are not very friendly for humans. For details, visit
#         https://secure.packetizer.com/pwgen/.
#
#     Parameters
#         length [in]
#             The length of the password
#         special [in]
#             Indicates whether to use special characters other than
#             the letters A-Z, a-z, and digits 0-9.
#
#     Returns
#         A string containing the password, or an empty string if there
#         was an error producing the password.
#
sub GeneratePassword
{
    my ($length, $special) = @_;
    my $password = "";

    my @pwchars = (
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
        'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z', '~', '`', '!', '@', '#', '$', '%', '^',
        '&', '*', '(', ')', '_', '+', '=', '-', '{', '}', '|', '\\', ']', '[',
        ':', '"', '\'', ';', '<', '>', '?', '/', '.'
    );

    while($length > 0)
    {
        if ($special == 1)
        {
            $password .= $pwchars[irand(93)];
        }
        else
        {
            $password .= $pwchars[irand(62)];
        }
        $length--;
    }

    return $password;
}

#
# MAIN
#
{
    my $password_length;
    my $use_special_characters = 0;

    #
    # Grab the requested password length from the command-line
    #
    if ($#ARGV >= 0)
    {
        $password_length = $ARGV[0];
        if (!($password_length > 0))
        {
            $password_length = $main::default_password_length;
        }
    }
    else
    {
        $password_length = $main::default_password_length;
    }

    #
    # Use special characters?
    #
    if ($#ARGV >= 1)
    {
        if ($ARGV[1] == 1)
        {
            $use_special_characters = 1;
        }
    }

    # We will not utilize special char
    print GeneratePassword($password_length,$use_special_characters) . "\n";
}

