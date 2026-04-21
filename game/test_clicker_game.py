"""Модуль тестов для игры «Бой с монстром»."""

import unittest
import tkinter as tk
from unittest.mock import patch
from clicker_game import ClickerGame


class TestClickerGame(unittest.TestCase):
    """Набор тестов для проверки игровой логики класса ClickerGame."""

    def setUp(self) -> None:
        """Создание экземпляра игры перед каждым тестом."""
        self.root = tk.Tk()
        self.game = ClickerGame(self.root)

    def tearDown(self) -> None:
        """Закрытие окна после каждого теста."""
        self.root.destroy()

    def test_initial_state(self) -> None:
        """Проверка начальных значений переменных."""
        self.assertEqual(self.game.coins, 0)
        self.assertEqual(self.game.click_power, 1)
        self.assertEqual(self.game.monster_level, 1)
        self.assertEqual(self.game.monster_max_health, 100)
        self.assertEqual(self.game.monster_health, 100)
        self.assertEqual(self.game.player_max_health, 100)
        self.assertEqual(self.game.player_health, 100)
        self.assertEqual(self.game.potion_cost, 20)
        self.assertEqual(self.game.potion_heal, 30)

    def test_attack_reduces_monster_health(self) -> None:
        """Атака уменьшает здоровье монстра и увеличивает монеты."""
        initial_monster_health = self.game.monster_health
        initial_coins = self.game.coins
        initial_player_health = self.game.player_health

        self.game.on_attack()

        self.assertLess(self.game.monster_health, initial_monster_health)
        self.assertGreater(self.game.coins, initial_coins)
        self.assertLess(self.game.player_health, initial_player_health)

    @patch.object(tk.Tk, 'after', return_value=None)
    def test_monster_defeat_spawns_new_monster(self, mock_after) -> None:
        """
        При смерти монстра появляется новый монстр с повышенным уровнем.
        Мокаем root.after, чтобы spawn_new_monster вызывался немедленно.
        """
        # Настраиваем мок так, чтобы он сразу вызывал переданную функцию
        def after_side_effect(delay, callback, *args):
            callback(*args)
        mock_after.side_effect = after_side_effect

        # Устанавливаем здоровье монстра близким к 0
        self.game.monster_health = 1
        level_before = self.game.monster_level

        self.game.on_attack()  # Должен убить монстра и вызвать spawn_new_monster

        self.assertGreater(self.game.monster_level, level_before)
        self.assertEqual(self.game.monster_health, self.game.monster_max_health)
        self.assertGreater(self.game.click_power, 1)

    def test_player_death_resets_game(self) -> None:
        """Если здоровье игрока падает до нуля, игра сбрасывается."""
        self.game.player_health = 1
        self.game.monster_health = 1000  # Чтобы монстр не умер

        self.game.on_attack()  # Игрок получает урон и умирает

        # После сброса должно быть начальное состояние
        self.assertEqual(self.game.coins, 0)
        self.assertEqual(self.game.player_health, self.game.player_max_health)
        self.assertEqual(self.game.monster_level, 1)

    def test_buy_potion_heals_player(self) -> None:
        """Покупка зелья восстанавливает здоровье и тратит монеты."""
        self.game.coins = 50
        self.game.player_health = 50

        self.game.buy_potion()

        self.assertEqual(self.game.coins, 30)
        self.assertEqual(self.game.player_health, 80)

    def test_buy_potion_not_enough_coins(self) -> None:
        """Зелье не покупается при недостатке монет."""
        self.game.coins = 10
        old_health = self.game.player_health

        self.game.buy_potion()

        self.assertEqual(self.game.coins, 10)
        self.assertEqual(self.game.player_health, old_health)

    def test_buy_potion_full_health(self) -> None:
        """При полном здоровье зелье не используется."""
        self.game.coins = 50
        self.game.player_health = self.game.player_max_health

        self.game.buy_potion()

        self.assertEqual(self.game.coins, 50)
        self.assertEqual(self.game.player_health, self.game.player_max_health)

    def test_reset_game(self) -> None:
        """Сброс возвращает все параметры к начальным."""
        # Изменяем состояние
        self.game.coins = 100
        self.game.monster_level = 5
        self.game.player_health = 30

        self.game.reset_game()

        self.assertEqual(self.game.coins, 0)
        self.assertEqual(self.game.monster_level, 1)
        self.assertEqual(self.game.player_health, self.game.player_max_health)


if __name__ == "__main__":
    unittest.main()