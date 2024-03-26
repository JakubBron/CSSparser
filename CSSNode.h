#pragma once
#include "DoublyLinkedList.h"
#include <iostream>


#define String DoublyLinkedList<char>
const int howMuch = 8; // array size (T in instruction)
const int neutralValue = -1;

struct section {
    DoublyLinkedList<String> selectors;
    DoublyLinkedList<String> properties;
    DoublyLinkedList<String> values;
    bool isUsed = false;

    int countSelectors()
    {
        return selectors.getLength();
    }

    int countProperties()
    {
        return properties.getLength();
    }

    String *getSelector(int number)
    {
        return selectors.get(number);
    }
};

struct sectionNode {
    section tab[howMuch];
    int fill = 0;
    int count = 0; // count - how much active in node?
};

class CSS {
private:
    DoublyLinkedList<sectionNode> input;
public: 
    void add(section &&toPush) 
    {
        Node<sectionNode>* tail = input.getTail();
        if (tail == NULL || tail->data.fill == howMuch)
            input.add(sectionNode());

        tail = input.getTail();
        tail->data.tab[tail->data.fill] = (move(toPush));
        tail->data.fill++;
        tail->data.count++;
    }

    void remove(const int position)
    {
        Node<sectionNode>* temp = input.getHead();
        int currPos = 0;
        int whichNode = 0;
        while (temp != NULL)
        {
            if (currPos < position && currPos + temp->data.count < position)
            {
                currPos += temp->data.count;
            }
            else
            {
                temp->data.tab[position - currPos - 1].selectors.removeAll();
                temp->data.tab[position - currPos - 1].properties.removeAll();
                temp->data.tab[position - currPos - 1].values.removeAll();
                temp->data.tab[position - currPos - 1].isUsed = false;
                temp->data.count--;
                temp->data.fill--;
                for (int i = position - currPos; i < howMuch; i++)
                {
                    temp->data.tab[i - 1] = move(temp->data.tab[i]);
                    temp->data.tab[i].isUsed = false;
                }
                if (temp->data.count == 0)
                    input.remove(whichNode);

                break;
            }
            whichNode++;
            temp = temp->next;
        }
    }

    int countSections()
    {
        int ans = 0;
        Node<sectionNode>* temp = input.getHead();
        while (temp != NULL)
        {
            ans += temp->data.count;
            temp = temp->next;
        }
        return ans;
    }

    int traverseAndFindAttribute(const String&& toFind)
    {
        int cnt = 0;
        Node<sectionNode>* temp = input.getHead();
        while (temp != NULL)
        {
            for (int i = 0; i < howMuch; i++)
            {
                Node<String>* propertiesNode = temp->data.tab[i].properties.getHead();
                while (propertiesNode != NULL)
                {
                    if (areEqual(toFind, propertiesNode->data))
                    {
                        cnt++;
                        break;
                    }
                    propertiesNode = propertiesNode->next;
                }
            }
            temp = temp->next;
        }
        return cnt;
    }

    int traverseAndFindSelector(const String&& toFind)
    {
        int cnt = 0;
        Node<sectionNode>* temp = input.getHead();
        while (temp != NULL)
        {
            for (int i = 0; i < howMuch; i++)
            {
                Node<String>* selectorsNode = temp->data.tab[i].selectors.getHead();
                while (selectorsNode != NULL)
                {
                    if (areEqual(toFind, selectorsNode->data))
                    {
                        cnt++;
                        break;
                    }
                    selectorsNode = selectorsNode->next;
                }
            }
            temp = temp->next;
        }
        return cnt;
    }

    section* findSection(const int Section)
    {
        if (Section < 0)
            return nullptr;

        Node<sectionNode>* temp = input.getHead();
        int cnt = 0;
        while (temp != NULL)
        {
            if (cnt + temp->data.count < Section)
            {
                cnt += temp->data.count;
            }
            else 
            {
                for (int i = 0; i < howMuch; i++)
                {
                    if (temp->data.tab[i].isUsed == true)
                    {
                        if (cnt + 1 < Section)
                            cnt++;
                        else
                            return &(temp->data.tab[i]);
                    }

                }
            }
            temp = temp->next;
        }
        return nullptr;
    }

    String* getLastValueOfParamInSelector(const String&& selector, const String&& parameter)
    {
        Node<sectionNode>* temp = input.getTail();
        while (temp != NULL)
        {
            for (int i = howMuch-1; i>=0; i--)
            {
                if (temp->data.tab[i].isUsed == false)
                    continue;

                Node<String>* checkedSelector = temp->data.tab[i].selectors.getTail();
                while (checkedSelector != NULL)
                {
                    if (areEqual(selector, checkedSelector->data))
                    {
                        Node<String>* checkedProperties = temp->data.tab[i].properties.getTail();
                        Node<String>* checkedValues = temp->data.tab[i].values.getTail();
                        while (checkedProperties != NULL)
                        {
                            if (areEqual(parameter, checkedProperties->data))
                            {
                                return &(checkedValues->data);
                            }
                            checkedProperties = checkedProperties->prev;
                            checkedValues = checkedValues->prev;
                        }
                    }
                    checkedSelector = checkedSelector->prev;
                }
            }
            temp = temp->prev;
        }
        return nullptr;
    }
};
