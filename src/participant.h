#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <iostream>
#include <QString>

enum ParticipantKind
{
    CHALLENGER,
    GAMEMASTER
};
class Participant
{
protected:
    QString id;
    QString password;
    int level;

public:
    Participant() = default;
    Participant(const QString &id, const QString &password, int level)
        : id(id), password(password), level(level) {}

    virtual ~Participant() {}

    const QString &getpassword() const
    {
        return password;
    }
    const QString &getid() const
    {
        return id;
    }
    void setLevel(int level)
    {
        this->level = level;
    }

    int getLevel() const
    {
        return level;
    }

    virtual void printDetails() const = 0;
};

class Challenger : public Participant
{
private:
    int passedLevels;
    int experience;

public:
    Challenger() = default;
    Challenger(const QString &id, const QString &password, int passedLevels, int experience, int level)
        : Participant(id, password, level), passedLevels(passedLevels), experience(experience) {}
    bool operator<(const Challenger &other) const
    {
        if (level == other.getLevel())
            return experience > other.getExperience();
        else
            return level > other.getLevel();
    }

    void setPassedLevels(int passedLevels)
    {
        this->passedLevels = passedLevels;
    }

    int getPassedLevels() const
    {
        return passedLevels;
    }

    void setExperience(int experience)
    {
        this->experience = experience;
    }

    int getExperience() const
    {
        return experience;
    }
    int addExperience(int num)
    {
        experience += num;
        if (experience >= 60)
        {
            experience -= 60;
            level++;
        }
    }
    void printDetails() const override
    {
        // std::cout << "Challenger id: " << id << std::endl;
        // std::cout << "Passed Levels: " << passedLevels << std::endl;
        // std::cout << "Experience: " << experience << std::endl;
        // std::cout << "Level: " << level << std::endl;
    }
};

class Quizmaster : public Participant
{
private:
    int qusetionsCounts;

public:
    Quizmaster() = default;
    Quizmaster(const QString &id, const QString &password, int qusetionsCounts, int level) : Participant(id, password, level), qusetionsCounts(qusetionsCounts) {}

    bool operator<(const Quizmaster &other) const
    {
        if (level == other.getLevel())
            return qusetionsCounts > other.getqusetionsCounts();
        else
            return level > other.getLevel();
    }
    void setqusetionsCounts(int qusetionsCounts)
    {
        this->qusetionsCounts = qusetionsCounts;
    }

    int getqusetionsCounts() const
    {
        return qusetionsCounts;
    }
    void incQusetionCounts()
    {
        qusetionsCounts++;
        level = qusetionsCounts / 10;
    }
    void printDetails() const override
    {
        // std::cout << "Quizmaster id: " << id << std::endl;
        // std::cout << "Questions Created: " << qusetionsCounts << std::endl;
        // std::cout << "Level: " << level << std::endl;
    }
};

#endif // PARTICIPANT_H