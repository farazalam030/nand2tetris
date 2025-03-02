#include "SymbolTable.h"

void SymbolTable::addSymbolsToSymbolTable(string symbol, int address) {
  if (SymbolAddressTable.find(symbol) == SymbolAddressTable.end()) {
    SymbolAddressTable.insert({symbol, address});
  }
}

bool SymbolTable::isPresent(string symbol) {
  return SymbolAddressTable.find(symbol) != SymbolAddressTable.end();
}

int SymbolTable::getAddressOfSymbol(string symbol) {
  if (SymbolAddressTable.find(symbol) != SymbolAddressTable.end())
    return SymbolAddressTable[symbol];
  else
    return NOTPRESENT;
}