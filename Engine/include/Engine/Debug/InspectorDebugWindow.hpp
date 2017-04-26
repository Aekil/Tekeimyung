/**
* @Author   Mathieu Chassara
*/

#include <Engine/Debug/DebugWindow.hpp>

typedef std::function<void(void)>   funcPtr;

class   InspectorDebugWindow : public DebugWindow
{
public:
    explicit    InspectorDebugWindow();
    virtual     ~InspectorDebugWindow();

    void        build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    template<typename T>
    void        bindPopulateFunction(std::shared_ptr<T> debugWindow);

    GENERATE_ID(InspectorDebugWindow);
private:
    funcPtr     _populateFuncPtr = nullptr;

    //template<typename T>
    //std::shared_ptr<T>  _debugWindow = nullptr;
};