#ifndef _s21_list_edit_
#define _s21_list_edit_

namespace s21 {

template <typename T>
void list<T>::clear() {
  while (!empty()) {  // пока не пусто
    pop_front();  // удаление по элементу с начала списка
  }
}

template <typename T>
void list<T>::pop_back() {
  if (!empty()) {                    // если не пустой список
    Node* to_delete = sentry->prev;  // указаение на узел для удаления
    sentry->prev = to_delete->prev;
    // передача соседнего указателя по следующему порядку
    // на предыдущий узел узла который в процессе удаления
    to_delete->prev->next = sentry;  // тот узел тоже аналогично
    delete_node(to_delete);          // удаление
    --list_size;  // уменьшение размера на 1
  }
}

template <typename T>
void list<T>::pop_front() {  // удаление с начала списка
  if (!empty()) {            // если не пустой список
    Node* for_delete = sentry->next;
    sentry->next =
        for_delete->next;  // передача узла на обращение к соседу через узел
    for_delete->next->prev = sentry;  // аналогично
    delete_node(for_delete);          // удаление
    --list_size;
  }
}
template <typename T>
void list<T>::push_back(const_reference value) {  // добавление к концу
  Node* new_node = create_node(value);            // создание узла

  new_node->next = sentry;  // инициализация соседа впереди
  new_node->prev = sentry->prev;  // сзади
  sentry->prev->next = new_node;  // соседи узнали нового соседа
  sentry->prev = new_node;

  ++list_size;
}

template <typename T>
void list<T>::push_front(
    const_reference value) {  // аналогично сверху только к началу
  Node* new_node = create_node(value);
  new_node->next = sentry->next;
  new_node->prev = sentry;
  sentry->next->prev = new_node;
  sentry->next = new_node;

  ++list_size;
}
// добавление узла по позиции
template <typename T>
typename list<T>::iterator list<T>::insert(list<T>::iterator pos,
                                           const_reference value) {
  Node* new_node = create_node(value);
  Node* pos_node = pos.current_ptr;

  new_node->next = pos.current_ptr;
  new_node->prev = pos_node->prev;
  pos_node->prev->next = new_node;
  pos_node->prev = new_node;

  ++list_size;
  return ListIterator(new_node);
}
// удаление узла
template <typename T>
void list<T>::erase(list<T>::ListIterator pos) {
  if (pos != end()) {
    Node* to_delete = pos.current_ptr;
    to_delete->prev->next = to_delete->next;
    to_delete->next->prev = to_delete->prev;
    delete_node(to_delete);
    --list_size;
  }
}
// смена содержимого между списками
template <typename T>
void list<T>::swap(list& other) {
  Node* temp_sentry = sentry;
  sentry = other.sentry;
  other.sentry = temp_sentry;

  size_type temp_size = list_size;
  list_size = other.list_size;
  other.list_size = temp_size;
}
// слияние двух списков
template <typename T>
void list<T>::merge(list& other) {
  if (this == &other ||
      other.empty()) {  // если копия или пустой второй, завершается
    return;
  }

  iterator this_it = begin();  // отсчет списка с начала
  iterator other_it = other.begin();
  iterator this_end = end();  // с конца и с другим
  iterator other_end = other.end();

  while (this_it != this_end && other_it != other_end) {
    if (*other_it <= *this_it) {
      Node* other_node = other_it.current_ptr;
      /*
      other_node сохраняет указатель на текущий узел
      из other (other_it.current_ptr), который будет перемещён в this
      */
      ++other_it;
      // перенаселение
      other_node->prev->next = other_node->next;
      other_node->next->prev = other_node->prev;
      /*
      Эти строки удаляют узел other_node из
       списка other, обновляя связи между соседними узлами
       Указатель next предыдущего узла теперь указывает
       на следующий узел, минуя other_node и так далее
      */
      other_node->next = this_it.current_ptr;
      other_node->prev = this_it.current_ptr->prev;
      this_it.current_ptr->prev->next = other_node;
      this_it.current_ptr->prev = other_node;
      /*
      Эти строки вставляют other_node в список this
       перед текущим узлом this_it.current_ptr
       Новый узел указывает вперёд на текущий узел this_it
        Новый узел указывает назад на предыдущий узел перед this_it
        Указатель next узла перед this_it теперь указывает на other_node
        Указатель prev текущего узла this_it теперь указывает на other_node
      */
      ++list_size;
      --other.list_size;
      // уменьшает размер списка other добавляя его в текущий
    } else {
      ++this_it;
      // если *other_it. > *this_it ничего не перемещается и дальше идет
      // сравнение
    }
  }

  if (other_it !=
      other_end) {  // остались ли непройденные элементы в списке other

    Node* last_node = sentry->prev;
    // указывает на последний узел текущего списка this. Этот узел будет
    // соединён с первым оставшимся узлом из other
    last_node->next = other_it.current_ptr;
    // Устанавливает указатель next последнего узла this (last_node) на первый
    // оставшийся узел из other
    other_it.current_ptr->prev = last_node;
    // Устанавливает указатель prev первого оставшегося узла из other на
    // последний узел this
    sentry->prev = other.sentry->prev;
    // Эта строка обновляет указатель prev фиктивного узла sentry в this, чтобы
    // он указывал на последний узел из other
    other.sentry->prev->next = sentry;
    // Это завершает интеграцию всех оставшихся узлов из other в this, замыкая
    // цепочку
    list_size += other.list_size;
    other.list_size = 0;
    // очистка списк other
    other.sentry->next = other.sentry;
    other.sentry->prev = other.sentry;
  }
}
// перемещение элементов начания с pos от списка other
template <typename T>
void list<T>::splice(const_iterator pos, list& other) {
  if (this == &other || other.empty()) {
    return;
  }

  Node* pos_node = pos.current_ptr;
  Node* other_first = other.sentry->next;
  Node* other_last = other.sentry->prev;

  other_first->prev = pos_node->prev;
  other_last->next = pos_node;
  pos_node->prev->next = other_first;
  pos_node->prev = other_last;

  list_size += other.list_size;
  other.list_size = 0;

  other.sentry->next = other.sentry;
  other.sentry->prev = other.sentry;
}

template <typename T>
void list<T>::reverse() {
  if (list_size <= 1) {
    return;
  }

  Node* current = sentry->next;
  Node* temp = nullptr;

  while (current != sentry) {
    /*
    Этот цикл проходит по всем узлам списка
    меняя местами указатели next и prev для каждого узла
    чтобы изменить направление связей
    */
    temp = current->next;
    current->next = current->prev;
    current->prev = temp;
    current = temp;
  }
  // После разворота всех узлов нужно обновить указатели sentry
  temp = sentry->next;
  sentry->next = sentry->prev;
  sentry->prev = temp;
}

// удаление дубликатов
template <typename T>
void list<T>::unique() {
  if (list_size <= 1) {
    return;
  }

  iterator it = begin();
  iterator next_it = it;
  ++next_it;

  while (next_it != end()) {
    if (*it == *next_it) {  // сравнение
      iterator to_erase = next_it;
      ++next_it;
      erase(to_erase);
    } else {  // проверка на следующую пару
      ++it;
      ++next_it;
    }
  }
}

template <typename T>
void list<T>::sort() {
  /*
    Деление списка пополам на временных полях списков
    с рекурсивной сортировкой друг на друга
    с последующим слиянием merge
  */
  if (list_size <= 1) {
    return;
  }

  list<T> left;
  list<T> right;
  size_type mid = list_size / 2;

  iterator it = begin();
  for (size_type i = 0; i < mid; ++i) {
    left.push_back(*it);
    ++it;
  }
  while (it != end()) {
    right.push_back(*it);
    ++it;
  }
  left.sort();
  right.sort();

  clear();

  merge(left);
  merge(right);
}
}  // namespace s21

#endif