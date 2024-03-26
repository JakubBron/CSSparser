#include <iostream>
#include "DoublyLinkedList.h"
#include "CSSNode.h"

using namespace std;

enum element_id {
	problemsToSolve = 1,
	selectors = 2,
	properties = 3,
	values = 4
};

const int maxArgsNumber = 3;

void dataClear(int argsInt[], String args[], short* argsCounter)
{
	argsInt[0] = neutralValue;
	argsInt[1] = neutralValue;
	argsInt[2] = neutralValue;
	args[0] = String();
	args[1] = String();
	args[2] = String();
	*argsCounter = 0;
}

int main()
{
	CSS css;
	String buffer, args[maxArgsNumber];
	int bufferAsInt = neutralValue, overwriteCounter = 0;
	int argsInt[maxArgsNumber] = { neutralValue, neutralValue, neutralValue };
	short argsCounter = 0;
	section newSection, trashcan;
	char t;
	element_id whatAmIreadingNow = selectors;
	element_id beforeChangeToProblemsMode = selectors;
	bool overwriteParam = false;

	while (t = cin.get())
	{
		if (t != '\n' && !feof(stdin))
		{
			if (t < ' ')
				continue;

			buffer.add(move(t));
		}
		if (t == '\n' && whatAmIreadingNow != problemsToSolve)
			continue;

		if (cin.eof() && buffer.getLength() > 0)
			t = '\n';

		if (cin.eof() && buffer.getLength() == 0)
			break;

		char last4chars[4] = { '`', '`', '`', '`' };
		Node<char>* temp = buffer.getTail();

		for (int i = 0; i < 4; i++)
		{
			if (temp != NULL)
			{
				last4chars[i] = temp->data;
				temp = temp->prev;
			}
			else break;
		}

		if (last4chars[0] == '?' && last4chars[1] == '?' && last4chars[2] == '?' && last4chars[3] == '?')
		{
			// problem solving mode
			beforeChangeToProblemsMode = whatAmIreadingNow;
			whatAmIreadingNow = problemsToSolve;
			
			// removing excess data from buffer
			for(int i=0; i<4; i++)
				buffer.remove();
			
			continue;
		}
		if (last4chars[0] == '*' && last4chars[1] == '*' && last4chars[2] == '*' && last4chars[3] == '*')
		{
			// continue reading CSS mode
			whatAmIreadingNow = beforeChangeToProblemsMode;
	
			// removing excess data from buffer
			for (int i = 0; i < 4; i++)
				buffer.remove();
			
			continue;
		}
		// reading & processing input
		if (whatAmIreadingNow != problemsToSolve) 
		{
			if ((buffer.getTailData() == '\t' || buffer.getTailData() == ' ') && buffer.getLength() == 1)
			{
				buffer.remove();
				continue;
			}

			if (whatAmIreadingNow == selectors && buffer.getTailData() == ',')
			{
				buffer.remove();
				newSection.selectors.add(move(buffer));
				newSection.isUsed = true;
				continue;
			}

			if (buffer.getTailData() == '{' && whatAmIreadingNow == selectors)
			{
				whatAmIreadingNow = properties;
				buffer.remove();
				if (buffer.getLength() > 0)
				{
					while (buffer.getTailData() == ' ' || buffer.getTailData() == '\t')
						buffer.remove();

					newSection.selectors.add(move(buffer));
					newSection.isUsed = true;
				}
				continue;
			}

			if (buffer.getTailData() == ':' && whatAmIreadingNow == properties)
			{
				buffer.remove();
				whatAmIreadingNow = values;
				if (buffer.getLength() > 0)
				{
					Node<String>* props = newSection.properties.getHead();
					while (props != NULL)
					{
						if (areEqual(props->data, buffer))
						{
							overwriteParam = true;
							newSection.properties.remove(overwriteCounter);
							newSection.values.remove(overwriteCounter);
							break;
						}
						props = props->next;
						overwriteCounter++;
					}
					if (overwriteParam == false)
						overwriteCounter = 0;

					newSection.properties.add(move(buffer));
					newSection.isUsed = true;
				}
				continue;
			}
			
			if (buffer.getTailData() == ';' && whatAmIreadingNow == values)
			{
				buffer.remove();
				whatAmIreadingNow = properties;
				if (buffer.getLength() > 0)
				{
					newSection.values.add(move(buffer));
					newSection.isUsed = true;
					if (overwriteParam == true)
					{
						overwriteParam = false;
						overwriteCounter = 0;
					}
				}
				continue;
			}

			// case when '}' after last value in section
			if (buffer.getTailData() == '}' && whatAmIreadingNow == values)
			{
				buffer.remove();
				if (buffer.getLength() > 0)
				{
					newSection.values.add(move(buffer));
					newSection.isUsed = true;
					if (overwriteParam == true)
					{
						overwriteParam = false;
						overwriteCounter = 0;
					}
				}
				
				whatAmIreadingNow = selectors;
				if (newSection.isUsed == true)
					css.add(move(newSection));
				
				else
					trashcan = move(newSection);
				
				continue;
			}
			
			if (buffer.getTailData() == '}')
			{
				buffer.remove();
				whatAmIreadingNow = selectors;
				newSection.isUsed = true;
				if (newSection.isUsed == true)
					css.add(move(newSection));
				
				else
					trashcan = move(newSection);

				continue;
			}
		}
		// reading, processing, solving problems
		else 
		{
			if (buffer.getLength() == 0)
				continue;

			bool isDigit = true;

			// read problem arguments
			if (t != '\n')
			{
				if (buffer.getTailData() == ',')
				{
					buffer.remove();
					argsCounter++;
					Node<char>* temp = buffer.getHead();
					while (temp != NULL)
					{
						if (temp->data >= '0' && temp->data <= '9' && isDigit == true)
						{
							if (argsInt[argsCounter - 1] == neutralValue)
							{
								argsInt[argsCounter - 1] = int(temp->data - '0');
							}
							else
							{
								argsInt[argsCounter - 1] *= 10;
								argsInt[argsCounter - 1] += int(temp->data - '0');
							}
						}
						else
						{
							argsInt[argsCounter - 1] = neutralValue;
							isDigit = false;
						}

						temp = temp->next;
					}
					args[argsCounter - 1] = move(buffer);
				}
			}
			// solve problem
			else 
			{
				if (argsCounter > 2)
				{
					dataClear(argsInt, args, &argsCounter);
					buffer.removeAll();
					continue;
				}

				isDigit = true;
				
				// ?
				if (buffer.getTailData() == '?' && args[0].getHead() == NULL)
				{
					cout << "? == " << css.countSections() << "\n";
					dataClear(argsInt, args, &argsCounter);
					buffer.removeAll();
					continue;
				}

				// process last arg
				if (buffer.getLength() != 0)
				{
					argsCounter++;
					Node<char>* temp = buffer.getHead();
					while (temp != NULL)
					{
						if (temp->data >= '0' && temp->data <= '9')
						{
							if (argsInt[argsCounter - 1] == neutralValue)
							{
								argsInt[argsCounter - 1] = int(temp->data - '0');
							}
							else
							{
								argsInt[argsCounter - 1] *= 10;
								argsInt[argsCounter - 1] += int(temp->data - '0');
							}
						}
						else
						{
							argsInt[argsCounter - 1] = neutralValue;
							break;
						}

						temp = temp->next;
					}
					args[argsCounter - 1] = move(buffer);

				}
				argsCounter = 0;

				
				// i,S,?
				if (argsInt[0] != neutralValue && args[1].getTailData() == 'S' && args[1].getLength() == 1 && args[2].getTailData() == '?' 
					&& args[2].getLength() == 1)
				{
					int result = neutralValue;
					section* found = css.findSection(argsInt[0]);
					if (found != nullptr)
						result = found->selectors.getLength();

					if (result >= 0)
					{
						cout << argsInt[0] << ",";
						args[1].printAll();
						cout << ",";
						args[2].printAll();
						cout << " == " << result << "\n";
					}
					dataClear(argsInt, args, &argsCounter);
					continue;
				}

				//i,A,?
				if (argsInt[0] != neutralValue && args[1].getTailData() == 'A' && args[1].getLength() == 1 && args[2].getTailData() == '?'
					&& args[2].getLength() == 1)
				{
					int result = neutralValue;
					section* found = css.findSection(argsInt[0]);
					if (found != nullptr)
						result = (*found).countProperties();

					if (result != neutralValue)
					{
						cout << argsInt[0] << ",";
						args[1].printAll();
						cout << ",";
						args[2].printAll();
						cout << " == " << result << "\n";
					}
					dataClear(argsInt, args, &argsCounter);
					continue;
				}

				//i,S,j
				if (argsInt[0] != neutralValue && args[1].getTailData() == 'S' && args[1].getLength() == 1 && argsInt[2] != neutralValue)
				{
					String* result = nullptr;
					section* found = css.findSection(argsInt[0]);
					if (found != nullptr)
					{
						result = (*found).getSelector(argsInt[2]);
						if (result != nullptr)
						{
							cout << argsInt[0] << ",S," << argsInt[2] << " == ";
							(*result).printAll();
							cout << "\n";
						}
					}
					dataClear(argsInt, args, &argsCounter);
					continue;
				}

				//i,A,n
				if (argsInt[0] != neutralValue && args[1].getTailData() == 'A' && args[1].getLength() == 1 && argsInt[2] == neutralValue 
					&& args[2].getLength() > 0)
				{
					section* found = css.findSection(argsInt[0]);
					if (found != nullptr)
					{
						Node<String>* temp = (*found).properties.getHead();
						Node<String>* tempValues = (*found).values.getHead();
						while (temp != NULL)
						{
							if (areEqual(temp->data, args[2]))
							{
								cout << argsInt[0] << ",A,";
								args[2].printAll();
								cout << " == ";
								tempValues->data.printAll();
								cout << "\n";
								break;
							}
							else
							{
								temp = temp->next;
								tempValues = tempValues->next;
							}
						}

					}
					dataClear(argsInt, args, &argsCounter);
					continue;

				}

				// n,A,?
				if (argsInt[0] == neutralValue && args[0].getLength() > 0 && args[1].getLength() == 1 && args[1].getTailData() == 'A' 
					&& args[2].getLength() == 1 && args[2].getTailData() == '?')
				{
					int result = 0;
					result = css.traverseAndFindAttribute(move(args[0]));
					args[0].printAll();
					cout << ",A,? == " << result << "\n";

					dataClear(argsInt, args, &argsCounter);
					continue;
				}


				// z,S,?
				if (argsInt[0] == neutralValue && args[0].getLength() > 0 && args[1].getLength() == 1 && args[1].getTailData() == 'S' 
					&& args[2].getLength() == 1 && args[2].getTailData() == '?')
				{
					int result = 0;
					result = css.traverseAndFindSelector(move(args[0]));
					args[0].printAll();
					cout << ",S,? == " << result << "\n";

					dataClear(argsInt, args, &argsCounter);
					continue;
				}

				// z,E,n
				if (argsInt[0] == neutralValue && args[0].getLength() > 0 && args[1].getLength() == 1 && args[1].getTailData() == 'E' 
					&& argsInt[2] == neutralValue && args[2].getLength() > 0)
				{
					String* answer = css.getLastValueOfParamInSelector(move(args[0]), move(args[2]));
					if (answer != nullptr)
					{
						args[0].printAll();
						cout << ",E,";
						args[2].printAll();
						cout << " == ";
						answer->printAll();
						cout << "\n";
					}
					dataClear(argsInt, args, &argsCounter);
					continue;
				}

				//i,D,*
				if (argsInt[0] != neutralValue && args[1].getLength() == 1 && args[1].getTailData() == 'D' && args[2].getLength() == 1 
					&& args[2].getTailData() == '*')
				{
					if (argsInt[0] <= css.countSections())
					{
						css.remove(argsInt[0]);
						cout << argsInt[0] << ",D,* == deleted\n";
					}
					dataClear(argsInt, args, &argsCounter);
					continue;
				}

				//i,D,n
				if (argsInt[0] != neutralValue && args[1].getLength() == 1 && args[1].getTailData() == 'D' && argsInt[2] == neutralValue 
					&& args[2].getLength() > 0)
				{
					section* foundSection = css.findSection(argsInt[0]);
					bool printOutput = false;
					if (foundSection != NULL)
					{
						Node<String>* foundProperty = foundSection->properties.getHead();
						Node<String>* foundValue = foundSection->values.getHead();
						int cnt = 0;
						while (foundProperty != NULL)
						{
							if (areEqual(args[2], foundProperty->data))
							{
								foundSection->properties.remove(cnt);
								foundSection->values.remove(cnt);
								if (foundSection->properties.getLength() == 0)
									css.remove(argsInt[0]);

								printOutput = true;
								break;
							}
							foundProperty = foundProperty->next;
							foundValue = foundValue->next;
							cnt++;
						}
					}
					if (printOutput == true)
					{
						cout << argsInt[0] << ",D,";
						args[2].printAll();
						cout << " == deleted\n";
					}

					dataClear(argsInt, args, &argsCounter);
					continue;
				}
				dataClear(argsInt, args, &argsCounter);
			}
		}

	}
	return 0;
}

