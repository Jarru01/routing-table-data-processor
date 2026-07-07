#pragma once

#include <complexities/complexity_analyzer.h>
#include <random>
#include <libds/adt/list.h>
#include <vector>
#include <libds/adt/table.h>

namespace ds::utils
{
    /**
     * @brief Common base for table analyzers.
     */
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(Table& structure, size_t size) override;
        size_t getRandomKey() const;
        int getRandomIndex() const;

        ds::adt::ImplicitList<int> insertedKeys_;
        std::default_random_engine rngIndex_;
        std::default_random_engine rngKey_;
        int key_;
        int index_;
        

    };

    /**
     * @brief Analyzes complexity of an insertion
     */
    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Analyzes complexity of finding an element
     */
    template<class Table>
    class TableFindAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Container for all table analyzers.
     */
    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    //----------

    template<class Table>
    TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer<Table>(name),
        rngIndex_(144),
        rngKey_(144),
        key_(0),
        index_(0),
		insertedKeys_()
    {
    }

    template <class Table>
    void TableAnalyzer<Table>::growToSize(Table& structure, size_t n)
    {   
        structure.clear();
        this->insertedKeys_.clear();
        std::uniform_int_distribution<int> keyDist(INT_MIN, INT_MAX);
        for (size_t i = 0; i < n; i++)
        {
            int key = keyDist(this->rngKey_);
            int data = key;

            try {
                structure.insert(key, data);
                this->insertedKeys_.insertLast(key);
            }
            catch (...) {
                i--;
            }
        }
    }

    template<class Table>
    size_t TableAnalyzer<Table>::getRandomKey() const
    {
        return key_;
    }

    template<class Table>
    int TableAnalyzer<Table>::getRandomIndex() const
    {
        return index_;
    }

    //----------

    template <class Table>
    TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
    {
        ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
            {  
                std::uniform_int_distribution<int> keyDist(INT_MIN, INT_MAX);
                this->key_ = keyDist(this->rngKey_);
            });
    }

    template <class Table>
    void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
    {
        structure.insert(this->getRandomKey(), this->getRandomKey());
    }

    //----------

    template <class Table>
    TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
    {
        ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
            {
                std::uniform_int_distribution<int> dist(0, this->insertedKeys_.size() - 1);
                this->key_ = this->insertedKeys_.access(dist(this->rngKey_));
            });
    }

    template <class Table>
    void TableFindAnalyzer<Table>::executeOperation(Table& structure)
    {
        structure.find(this->getRandomKey());
    }

    //----------

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
    {
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("sorted-sequence-insert"));
        this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("sorted-sequence-find"));

    }
}
