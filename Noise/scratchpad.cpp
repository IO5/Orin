#include <variant>
#include <string_view>
#include <optional>

template <typename ValueType, typename CharT = char>
class Trie {
    using string_view = std::basic_string_view<CharT>;
public:
    bool insert(string_view str, ValueType&& value) { 
        std::vector<NodeOrLeaf>* current = root&;

        auto it = current->begin();
        while (it != current->end()) {
            auto& child = *it;

            string_view nodeStr = child.getStr();
            auto mismatchIt = std::mismatch(str.begin(), str.end(), nodeStr.begin(), nodeStr.end());
            size_t mismatchOff = std::distance(str.begin, mismatchIt.first);

            // exact match
            if (mismatchOff == str.size() && nodeStr.size() == str.size()) {
                if (child.isLeaf()) {
                    // duplicate
                    return false;
                } else {
                    // continue down the tree
                    current = &(std::get<Node>(child).children);
                    it = current.begin();
                    str = str.substr(mismatchOff);
                }
            // common prefix
            } else if (mismatchOff != 0) {
                // extract prefix into new node parenting new leaf and the old node
                string_view commonPrefix = str.substr(0, mismatchOff);
                string_view reminder = str.substr(mismatchOff);

                Leaf newLeaf{reminder, 0, std::forward<ValueType>(value)};
                Node newNode{commonPrefix, {std::move(child), std::move(newLeaf)}};
                child = std::move(newNode);
                return true;
            }

            ++it;
        }
        // no common prefix -> new leaf
        current->emplace_back(Leaf{str, 0, std::forward<ValueType>(value)});
        return true;
    }

    bool erase(Leaf& leaf) { 
        // replace parent
        if (leaf.parent->children.size() == 2) {
            otherChild;
            leaf.str.resize(leaf.off);
            if (leaf.str.size() + otherChild.size() > leaf.str.capacity()) {

            } else {
                leaf.str.append(other);
            }
        // merge
        } else {

        }

        std::vector<NodeOrLeaf>* current = root&;

        auto it = current->begin();
        while (it != current->end()) {
            auto& child = *it;

            string_view nodeStr = child.getStr();
            auto mismatchIt = std::mismatch(str.begin(), str.end(), nodeStr.begin(), nodeStr.end());
            size_t mismatchOff = std::distance(str.begin, mismatchIt.first);

            // exact match
            if (mismatchOff == str.size() && nodeStr.size() == str.size()) {
                if (child.isLeaf()) {
                    // duplicate
                    return false;
                } else {
                    // continue down the tree
                    current = &(std::get<Node>(child).children);
                    it = current.begin();
                    str = str.substr(mismatchOff);
                }
            // common prefix
            } else if (mismatchOff != 0) {
                // extract prefix into new node parenting new leaf and the old node
                string_view commonPrefix = str.substr(0, mismatchOff);
                string_view reminder = str.substr(mismatchOff);

                Leaf newLeaf{reminder, 0, std::forward<ValueType>(value)};
                Node newNode{commonPrefix, {std::move(child), std::move(newLeaf)}};
                child = std::move(newNode);
                return true;
            }

            ++it;
        }
        // no common prefix -> new leaf
        current->emplace_back(Leaf{str, 0, std::forward<ValueType>(value)});
        return true;
    }


private:
    struct Leaf {
        std::vector<CharT> str;
        size_t off;
        ValueType value;
    };

    struct NodeOrLeaf;

    struct Node {
        string_view str;
        std::vector<NodeOrLeaf> children;
    };

    struct NodeOrLeaf : public std::variant<Node, Leaf> {
        using std::variant<Node, Leaf>::variant;

        string_view getStr() {
            return std::visit(overload{
                [&](Node& node) { return node.str; },
                [&](Leaf& leaf) { return string_view{leaf.str.data(), leaf.str.size()}.substr(leaf.off); }
            }, *this);
        }

        bool isLeaf() {
            return std::visit(overload{
                [&](Node& node) { return false; },
                [&](Leaf& leaf) { return true; }
            }, *this);
        }
    };

    template<class... Ts>
    struct overload : Ts... { using Ts::operator()...; };

    std::vector<NodeOrLeaf> root;
};

