#ifndef _s21_list_cap_
#define _s21_list_cap_

namespace s21 {
// возвращает размер списка
template <typename T>
typename list<T>::size_type list<T>::size() {
  return list_size;
}
// проверка на пустой список
template <typename T>
bool list<T>::empty() {
  return list_size == 0;
}
// возвращает максимум размера списка
template <typename T>
typename list<T>::size_type list<T>::max_size() {
  return std::numeric_limits<size_type>::max() / sizeof(Node);
}

}  // namespace s21

#endif