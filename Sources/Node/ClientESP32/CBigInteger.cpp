#include "CBigInteger.h" 
 

bool CBigInteger::isLegal(){
	bool isLegal;
	if (str_int.length() == 1){
		isLegal = str_int.find_first_of(legalDigit.substr(0,legalDigit.length()-1)) != std::string::npos;  
		return isLegal;
	}
	else{ 
		isLegal = true;
		isLegal &= (legalDigit.find(str_int.at(0))) != std::string::npos ;
		isLegal &= (str_int.substr(1).find_first_not_of(legalDigit.substr(0, legalDigit.length() - 1)) == std::string::npos);

		return isLegal;

	}
}

bool CBigInteger::isZero(){

	
	if (str_int.find_first_not_of("0") == std::string::npos)
		return true;

	else if (str_int.at(0) == '-' && str_int.substr(1).find_first_not_of("0") == std::string::npos)
		return true;

		return false;
}

int CBigInteger::cmp(const CBigInteger& b) const{
	
	int thisLength = str_int.length();
	int cmpLength = b.GetInteger().length();

	if (thisLength > cmpLength) return 1;
	else if (thisLength < cmpLength) return -1;
	else {
		for (int i = 0; i < thisLength; i++){
			
			if (str_int.at(i) < b.GetInteger().at(i)) return -1;
			else if (str_int.at(i) > b.GetInteger().at(i)) return 1;
		}
		
		return 0;
	}
}

void CBigInteger::simplifyZero(){
	unsigned int index = 0;
	std::string result = "";

	if (isZero()){
		str_int = "0";
		return ;
	}

	else if (str_int.at(0) == '-'){
		index = 1;
		result = "-";
		while (index < str_int.length() && str_int.at(index) == '0'){
			index++;
		}
		result.append(str_int.substr(index));
		str_int = result;

		return ;
	}
	else {
		index = 0;
		while (index < str_int.length() && str_int.at(index) == '0'){
			index++;
		}
		result = str_int.substr(index);
		str_int = result;

		return ;
	}
}


CBigInteger::CBigInteger(const CBigInteger& b){
	legalDigit = "0123456789-";
	str_int = b.GetInteger();
	simplifyZero();

}

CBigInteger::CBigInteger(const char* pstr):str_int(pstr){
	legalDigit = "0123456789-";
	if (str_int.empty()){
		str_int = "0";
	}
	else if (!isLegal()){
		throw CBigIntegerError("woot");
	}

	simplifyZero();
}

CBigInteger::CBigInteger(const std::string& str) :str_int(str){
legalDigit = "0123456789-"; 
	if (str_int.empty()){
		str_int = "0";
	}
	else if (!isLegal()){
		throw CBigIntegerError("woot");
	}
	
	simplifyZero();
}


CBigInteger CBigInteger::addPos(const CBigInteger& b) const{
	std::string result;
	int firstIndex = str_int.length() - 1;
	int secondIndex = b.GetInteger().length() - 1;

	int firstDigit, secondDigit;
	int sum = 0, carry = 0;

	while (firstIndex >= 0 && secondIndex >= 0){
		
		firstDigit = CHAR_TO_DIGIT(str_int.at(firstIndex));
		secondDigit = CHAR_TO_DIGIT(b.GetInteger().at(secondIndex));

		sum = (firstDigit + secondDigit + carry) % 10;
		carry = (firstDigit + secondDigit + carry) / 10;
		

		result.append(std::string(1,DIGIT_TO_CHAR(sum)));

		firstIndex--;
		secondIndex--;

	}

	while (firstIndex >= 0){
		if (carry != 0){
			firstDigit = CHAR_TO_DIGIT(str_int.at(firstIndex));
			sum = (firstDigit + carry) % 10;
			carry = (firstDigit + carry) / 10;
			
		}
		else{
			sum = CHAR_TO_DIGIT(str_int.at(firstIndex));
		}
		result.append(std::string(1, DIGIT_TO_CHAR(sum)));
		firstIndex--;
	}

	while (secondIndex >= 0){
		if (carry != 0){
			secondDigit = CHAR_TO_DIGIT(b.GetInteger().at(secondIndex));
			sum = (secondDigit + carry) % 10;
			carry = (secondDigit + carry) / 10;

		}
		else{
			sum = CHAR_TO_DIGIT(b.GetInteger().at(secondIndex));
		}
		result.append(std::string(1, DIGIT_TO_CHAR(sum)));
		secondIndex--;
	}

	if (carry != 0){
		result.append(std::string(1, DIGIT_TO_CHAR(carry)));
	}

	reverse(result.begin(), result.end());
	
	return CBigInteger(result);
	
}



CBigInteger CBigInteger::subPos(const CBigInteger& b) const{

	if (cmp(b) == 0){
		return CBigInteger("0");
	}

	std::string result = "";
	int difference;
	int carry = 0;
	int firstIndex, secondIndex;
	int firstDigit, secondDigit;
	std::string firstNum, secondNum;

	if (cmp(b) == 1){
		firstNum = str_int;
		secondNum = b.GetInteger();
	}
	else if (cmp(b) == -1){
		firstNum = b.GetInteger();
		secondNum = str_int;
	}
	
	firstIndex = firstNum.length() - 1;
	secondIndex = secondNum.length() - 1;

	while (firstIndex >= 0 && secondIndex >= 0){
		
		firstDigit = CHAR_TO_DIGIT(firstNum.at(firstIndex));
		secondDigit = CHAR_TO_DIGIT(secondNum.at(secondIndex));

		difference = firstDigit - secondDigit - carry;
		
		if (difference < 0){
			difference += 10;
			carry = 1;
		}
		else {
			carry = 0;
		}

		result.append(std::string(1, DIGIT_TO_CHAR(difference)));
		
		firstIndex--;
		secondIndex--;

	}

	while (firstIndex >= 0){
		if (carry > 0){
			firstDigit = CHAR_TO_DIGIT(firstNum.at(firstIndex));
			difference = firstDigit - carry;
			
			if (difference >= 0) carry = 0;
			else {
				carry = 1;
				difference += 10;
			}

			result.append(std::string(1, DIGIT_TO_CHAR(difference)));
			
		}
		else{
			result.append(std::string(1, firstNum.at(firstIndex)));
			
		}
		firstIndex--;
	}

	reverse(result.begin(), result.end());

	if (cmp(b) == 1){
		return CBigInteger(result);
	}
	else {
		return -CBigInteger(result);
	}

}

CBigInteger CBigInteger::mulPos(const CBigInteger& b) const{
	if (str_int == "0" || b.GetInteger() == "0"){
		return CBigInteger("0");
	}

	CBigInteger result("0");
	std::string temp_result;


	int carry = 0;
	int product = 0;
	int digit1, digit2;
	int multiplierIndex, multiplicandIndex;

	for (multiplierIndex = str_int.length() - 1; multiplierIndex >= 0; multiplierIndex--){
		temp_result = "";
		carry = 0;
		
		for (multiplicandIndex = b.GetInteger().length() - 1; multiplicandIndex >= 0; multiplicandIndex--){
			digit1 = CHAR_TO_DIGIT(str_int.at(multiplierIndex));
			digit2 = CHAR_TO_DIGIT(b.GetInteger().at(multiplicandIndex));

			product = (digit1 * digit2 + carry) % 10;
			carry = (digit1 * digit2 + carry) / 10;
			
			
			temp_result.append(std::string(1,DIGIT_TO_CHAR(product)));
		}
		
		if (carry != 0){
			temp_result.append(std::string(1,DIGIT_TO_CHAR(carry)));
		}
		reverse(temp_result.begin(), temp_result.end());

		temp_result.append(std::string(str_int.length() - 1 - (multiplierIndex) , '0'));
		
		result = result.addPos(CBigInteger(temp_result));
	}
	result.simplifyZero();
	return result;

}

CBigInteger CBigInteger::divPos(const CBigInteger& b)const{

	if (b.GetInteger() == "0"){
		throw CBigIntegerError("Divisor cannot be zero!");
	}
	
	if (cmp(b) == 0){
		return CBigInteger("1");
	}
	else if (cmp(b) == -1){
		return CBigInteger("0");
	}
	else{
		int count = 0;
		CBigInteger temp;
	    std::string remainder;
		int dividendIndex = 0;
		std::string result;
		while (dividendIndex < str_int.length() ){
			
			count = 0;
			remainder.append(std::string(1, str_int.at(dividendIndex)));
			
			if (CBigInteger(remainder).cmp(b) < 0){
				
				result.append(std::string(1, DIGIT_TO_CHAR(count)));
				dividendIndex++;
				continue;
			}
			else{
				temp = CBigInteger(remainder);
				while (temp.cmp(b) >= 0){
					count++;
					temp = temp.subPos(b);
				}
				remainder = temp.GetInteger();
				/*remainder.append(std::string(1, str_int.at(dividendIndex)));*/
				
			}
			
			result.append(std::string(1, DIGIT_TO_CHAR(count)));
			dividendIndex++;
			

		}
	
	

		return CBigInteger(result);
	}
}


CBigInteger CBigInteger::modPos(const CBigInteger& b) const{

	if (b.GetInteger() == "0"){
		throw CBigIntegerError("Divisor cannot be zero!");
	}

	if (cmp(b) == 0){
		return CBigInteger("0");
	}
	else if (cmp(b) == -1){
		return CBigInteger(str_int);
	}
	else{
		int count = 0;
		CBigInteger temp;
		std::string remainder;
		int dividendIndex = 0;

		while (dividendIndex < str_int.length()){

			count = 0;
			remainder.append(std::string(1, str_int.at(dividendIndex)));

			if (CBigInteger(remainder).cmp(b) < 0){


				dividendIndex++;
				continue;
			}
			else{
				temp = CBigInteger(remainder);
				while (temp.cmp(b) >= 0){
					count++;
					temp = temp.subPos(b);
				}
				remainder = temp.GetInteger();


			}

			dividendIndex++;
		}
		return CBigInteger(remainder);
	}
}


CBigInteger CBigInteger::operator-()const{

	std::string result = str_int;

	if (result.at(0) == '-'){
		result.erase(0, 1);
	}
	else{
		result.insert(0, std::string(1, '-'));
	}
	return CBigInteger(result);
}

CBigInteger CBigInteger:: operator =(const CBigInteger& b){
	
	str_int.assign(b.GetInteger());
	
	return *this;
}

CBigInteger CBigInteger:: operator =(const std::string& str){
	
	if (!CBigInteger(str).isLegal()){
		throw CBigIntegerError("??");
	}

	str_int.assign(str);
	simplifyZero();

	return *this;

}
 
CBigInteger CBigInteger::pow(const CBigInteger& b) const{

	if (b.cmp(CBigInteger("0")) < 0){
		throw CBigIntegerError("The exp cannot be negative!");
	}

	CBigInteger result("1");
	CBigInteger count(b.GetInteger());

	std::string bi = "";
	CBigInteger remainder;

	while (count.cmp(CBigInteger("0")) > 0){

		remainder = count.modPos(CBigInteger("2"));
		
		bi.append(std::string(1,remainder.GetInteger().at(0)));
		
		count = count.divPos(CBigInteger("2"));
		
	}

	reverse(bi.begin(),bi.end());

	bool bitTrue;

	for (int i = 0; i < bi.length(); i++){
		
		result = result.mulPos(result);

		bitTrue = (bi.at(i) == '1');

		if (bitTrue)
		result = result.mulPos(str_int);

		

	}


	
	return result;

}

CBigInteger CBigInteger::powMod(const CBigInteger& b, const CBigInteger& m) const{

	if (b.cmp(CBigInteger("0")) < 0){
		throw CBigIntegerError("The exp cannot be negative!");
	}

	CBigInteger result("1");
	CBigInteger count(b.GetInteger());

	std::string bi = "";
	CBigInteger remainder;

	while (count.cmp(CBigInteger("0")) > 0){

		remainder = count.modPos(CBigInteger("2"));

		bi.append(std::string(1, remainder.GetInteger().at(0)));

		count = count.divPos(CBigInteger("2"));

	}

	reverse(bi.begin(), bi.end());

	bool bitTrue;

	for (int i = 0; i < bi.length(); i++){

		result = result.mulPos(result);

		result = result.modPos(m);

		bitTrue = (bi.at(i) == '1');

		if (bitTrue){
			result = result.mulPos(str_int);
			result = result.modPos(m);
		}


	}

	return result;

}

 

int CHAR_TO_DIGIT(char x){

	return  (int)x - 0x30;

}

char DIGIT_TO_CHAR(int x){

	return (char)(x + 0x30);

}
