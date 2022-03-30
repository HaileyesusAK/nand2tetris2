#include <unordered_set>
#include <sstream>
#include "expression.hpp"
#include "term_factory.hpp"

namespace ntt {

    /*
        expression : term (op term)*
        op : '+' |  '-' |  '*' |  '/' |  '&' |  '|' |  '<' |  '>' |  '='
    */
    Expression::Expression(Tokenizer& tokenizer) {
        static const std::unordered_set<std::string> ops {
            "+" ,  "-" ,  "*" ,  "/" ,  "&" ,  "|" ,  "<" ,  ">" ,  "="
        };

        terms_.emplace_back(TermFactory::parse(tokenizer));

        while(tokenizer.has_token()) {
            const auto& token = tokenizer.peek();
            if(!ops.count(token.value()))
                break;
            ops_.emplace_back(tokenizer.get());
            terms_.emplace_back(TermFactory::parse(tokenizer));
        }
    }

    std::string Expression::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<expression>", level);
        oss << terms_[0]->to_xml(level + 1);
        for(size_t i = 0; i < ops_.size(); ++i) {
            oss << JackFragment::to_xml(ops_[i], level + 1);
            oss << terms_[i+1]->to_xml(level + 1);
        }
        oss << JackFragment::get_line("</expression>", level);

        return oss.str();
    }

    const std::vector<std::unique_ptr<Term>>& Expression::terms() const {
        return terms_;
    }
    
    const std::vector<Token>& Expression::ops() const {
        return ops_;
    }

}
