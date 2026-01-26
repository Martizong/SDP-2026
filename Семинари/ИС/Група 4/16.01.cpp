#include <iostream>
#include <vector>
#include <queue>
#include <stack>

enum class Entity {
	WALL,
	ROAD,
	ARMOR,
	PLAYER,
	CREATURE,
	BLESSING
};

enum class EntitySubType {
	MINOTAVAR,
	HIMERA,
	HYDRA,
	ARES,
	ATHENA,
	APOLLO
};

template <typename T>
class CapacityElemCountingQueue {
public:
	void setCapacity(int newCapacity) {}
	void push(T elem) {}
	int size() { return 0; }
	void pop();
	int getElemCount(T elem) {
		return 1;
	}
	bool empty() {
		return false;
	}
	T front() {
		return T();
	}
};

/*



	void applyBlessing(EntitySubType subType) {
		switch (subType)
		{
		case EntitySubType::MINOTAVAR:
			break;
		case EntitySubType::HIMERA:
			break;
		case EntitySubType::HYDRA:
			break;
		case EntitySubType::ARES:
			break;
		case EntitySubType::ATHENA:
			break;
		case EntitySubType::APOLLO:
			break;
		default:
			break;
		}
	}
*/
struct Hero {
	int health;
	int attack;
	int armor;

	void applyBlessing(EntitySubType subType) {
		switch (subType)
		{
		case EntitySubType::ARES:
			attack += 50;
			break;
		case EntitySubType::ATHENA:
			attack *= 1.5;
			break;
		case EntitySubType::APOLLO:
			health *= 1.3;
			break;
		default:
			break;
		}
	}

	void fightAgainst(EntitySubType subType, int numCreatures) {
		int creatureAttack = 0, creatureHealth = 0, heroArmor = armor;
		switch (subType)
		{
		case EntitySubType::MINOTAVAR:
			creatureAttack = 150;
			creatureHealth = 150;
			break;
		case EntitySubType::HIMERA:
			creatureAttack = 250;
			creatureHealth = 350;
			break;
		case EntitySubType::HYDRA:
			creatureAttack = 200;
			creatureHealth = 200;
			break;
		default:
			break;
		}

		creatureAttack *= numCreatures;

		int healthToReplenish = 0.1 * creatureHealth * numCreatures;

		while (creatureHealth > 0 && health > 0) {
			creatureHealth -= attack;
			if (creatureHealth > 0) {
				int attackStrength = creatureAttack;
				if (heroArmor > 0) {
					attackStrength -= heroArmor;
				}
				if (attackStrength > 0) {
					health -= attackStrength;
				}
			}
		}

		if (health > 0) {
			health += healthToReplenish;
		}
	}
};

class Graph {
private:
	Hero hero;
	std::vector<std::vector<int>> adj;
	std::vector<Entity> nodes;
	std::vector<EntitySubType> nodesSubTypes;
	CapacityElemCountingQueue<EntitySubType> astralDimention;
	std::stack<EntitySubType> backpackWithBlessings;

	int getCurrentPos() {};
	std::vector<int> getNonVisitedNei(int current, std::vector<std::pair<int, int>> visited) {};
	std::vector<int> constructPath(int current, std::vector<std::pair<int, int>> visited, std::queue<std::pair<int, int>> q) {};


	void printPathItemSubType(int index) {
		EntitySubType mainEntityType = nodesSubTypes[index];
		switch (mainEntityType) {
			case EntitySubType::MINOTAVAR: std::cout << "MINOTAUR"; break;
			case EntitySubType::HIMERA: std::cout << "CHIMERA"; break;
			case EntitySubType::ATHENA: std::cout << "ATHENA"; break;
		}
	}

	void printPathItem(int index) {
		Entity mainEntityType = nodes[index];
		switch (mainEntityType) {
			case Entity::ROAD: std::cout << " "; break;
			case Entity::CREATURE:
			case Entity::BLESSING: printPathItemSubType(index); break;
		}
	}

	void printPathInfo(int pathOption, std::vector<int> path) {
		std::cout << "PATH OPTION: #" << pathOption;
		for (int nodeIndex : path) {
			printPathItem(nodeIndex);
			std::cout << "|";
		}
	}

	void addCreatureToAstral(EntitySubType creature) {
		astralDimention.push(creature);
	}

	void processPath(std::vector<int> path) {
		for (int nodeIndex: path) {
			switch (nodes[nodeIndex]) {
			case Entity::BLESSING: this->backpackWithBlessings.push(nodesSubTypes[nodeIndex]); break;
			case Entity::CREATURE: addCreatureToAstral(nodesSubTypes[nodeIndex]); break;
			}
			nodes[nodeIndex] = Entity::ROAD;
		}
	}

	void generateLabyrinth() {
		// we'll use rand to fill in nodes, nodesSubTypes and adj
	}

	std::vector<int> getPath(std::vector<int> &found){
		std::vector<int> path;
		std::vector<std::pair<int, int>> visited;
		std::queue<std::pair<int, int>> q;

		int startPos = getCurrentPos();
		q.push(std::pair<int, int>(startPos, startPos));
		
		bool pathFound = false;
		while (!q.empty() && !pathFound) {
			std::pair<int, int> current = q.front();
			q.pop();
			
			visited.push_back(current);
			std::vector<int> currentNei = getNonVisitedNei(current.first,visited);
			for (int el : currentNei) {
				q.push(std::pair<int, int>(el, current.first));
			}
			if (nodes[current.first] == Entity::ARMOR) {
				path = constructPath(current.first, visited, q);
				pathFound = true;
			}

		}

		return path;
	}
public:
	Graph() {
		this->astralDimention.setCapacity(5);
		hero.armor = 0;
		hero.health = 300;
		hero.attack = 50;
	}
	void askUserToChoose() {
		std::vector<int> found;
		std::vector<int> pathOne = getPath(found);
		std::vector<int> pathTwo = getPath(found);
		std::vector<int> pathThree = getPath(found);
		printPathInfo(1, pathOne);
		printPathInfo(2, pathTwo);
		printPathInfo(3, pathThree);
		int choice;
		std::cout << "Choose your path!";
		std::cin >> choice;
		if (choice == 1) {
			processPath(pathOne);
		}
		if (choice == 3) {
			processPath(pathTwo);
		}
		if (choice == 3) {
			processPath(pathThree);
		}
		generateLabyrinth();
	}

	void applyBlessings() {
		while (!this->backpackWithBlessings.empty()) {
			EntitySubType subType = this->backpackWithBlessings.top();
			hero.applyBlessing(subType);
			this->backpackWithBlessings.pop();
		}
	}

	void fight() {
		while (!this->astralDimention.empty()) {
			EntitySubType subType = this->astralDimention.front();
			hero.fightAgainst(subType, this->astralDimention.getElemCount(subType));
			this->astralDimention.front();
		}
	}

	void printResult() {
		if (hero.health > 0) {
			std::cout << "IMMORTAL!";
		}
		else {
			std::cout << "SORRY, YOU ARE DEAD!";
		}
	}
};

int main()
{
	Graph g;
	for (int i = 0; i < 5; i++) {
		g.askUserToChoose();
	}
	g.applyBlessings();
	g.fight();
	g.printResult();
}

