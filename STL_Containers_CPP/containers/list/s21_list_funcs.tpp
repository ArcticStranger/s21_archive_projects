
namespace s21 {
// empty constructor
template <typename T>
list<T>::list() : list_size(0) {
  sentry = new Node();    // выделение памяти для узла
  sentry->next = sentry;  // обращение на самого себя не имея соседнего узла
  sentry->prev = sentry;
}

template <typename T>
list<T>::list(size_type n) : list() {  // инициализация пустого конструктора
  for (size_type i = 0; i < n; ++i) {
    push_back(T());  // добавление к концу списка исходя из размера n
  }
}

template <typename T>
list<T>::list(std::initializer_list<value_type> const& items) : list_size(0) {
  sentry = new Node();
  sentry->next = sentry;
  sentry->prev = sentry;
  // указание диапазона через ": items"
  // и вставкой значений со списка инициалзиации через item
  // const со ссылкой что не именяется item для безопасности
  for (const auto& item : items) {
    push_back(item);
  }
}

// копирование элементов со списка l
template <typename T>
list<T>::list(const list& l) : list() {
  for (const auto& item : l) {
    push_back(item);
  }
}

// перемещение содержимого со списка other при этом устанавливая пустой other
template <typename T>
list<T>::list(list&& other) : sentry(other.sentry), list_size(other.list_size) {
  other.sentry = new Node();
  other.sentry->next = other.sentry;
  other.sentry->prev = other.sentry;
  other.list_size = 0;
}

// оператор присваивания
template <typename T>
list<T>& list<T>::operator=(list&& l) noexcept {
  if (this != &l) {  // если не обращается список на самого себя или копию
    clear();
    delete sentry;
    sentry = l.sentry;
    list_size = l.list_size;
    l.sentry = new Node();
    l.sentry->next = l.sentry;
    l.sentry->prev = l.sentry;
    l.list_size = 0;
  }
  return *this;
}
// деструктор списка
template <typename T>
list<T>::~list() {
  clear();
  delete sentry;
}

};  // namespace s21
