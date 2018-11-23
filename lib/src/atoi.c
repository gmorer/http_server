int ft_atoi(const char *str)
{
    int rslt;
    int i;
    int sign;

    rslt = 0;
    i = 0;
    sign = 1;
    while (str[i] >= 1 && str[i] <= 32)
        i++;
    if (str[i] == '+')
        i++;
    else if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    while (str[i] != '\0' && (str[i] >= '0' && str[i] <= '9'))
    {
        rslt = rslt * 10 + str[i] - '0';
        i++;
    }
    return (rslt * sign);
}